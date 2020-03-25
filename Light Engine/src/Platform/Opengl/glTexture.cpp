#include "ltpch.h"
#include "glTexture.h"

#include "Utility/FileManager.h"

#include <glad/glad.h>

namespace Light {

	glTextureArray::glTextureArray(unsigned int slices)
	{
		// set depth and available slots
		m_Depth = slices;
		for (int i = slices; i > 0; i)
			m_AvailableSlots.emplace_back(--i);


		// create texture array
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_ArrayID);

		glTextureParameteri(m_ArrayID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_ArrayID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ArrayID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ArrayID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTextureArray::~glTextureArray()
	{
		glDeleteTextures(1, &m_ArrayID);
	}

	void glTextureArray::CreateAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		// load texture
		TextureData data;
		data.pixels = FileManager::LoadTextureFile(texturePath, &data.width, &data.height, &data.channels);
		
		// first atlas? initialize width & height and specify the storage requirements
		if (!m_Width)
		{
			m_Width = data.width;
			m_Height = data.height;

			glTextureStorage3D(m_ArrayID, std::log(std::max(m_Width, m_Height)) + 1, GL_RGBA8, m_Width, m_Height, m_Depth);
		}


		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "glTextureArray::CreateAtlas: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "glTextureArray::CreateAtlas: failed to load texture file: {}", texturePath)

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "glTextureArray::CreateAtlas: texture dimension do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);


		// write to the texture array and generate mips
		glTextureSubImage3D(m_ArrayID, 0, 0, 0, m_AvailableSlots.back(), m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data.pixels);
		glGenerateTextureMipmap(m_ArrayID);


		// create atlas
		m_Atlases[name] = std::make_shared<TextureAtlas>(name, atlasPath, m_AvailableSlots.back());
		m_AvailableSlots.pop_back();
	}

	void glTextureArray::DeleteAtlas(const std::string& name)
	{
		if (m_Atlases[name])
			m_AvailableSlots.push_back(m_Atlases[name]->GetSliceIndex());
		else
			LT_CORE_ERROR("glTextureArray::DeleteAtlas: failed to find texture atlas: {}", name);

		m_Atlases.erase(name);
	}

	void glTextureArray::Bind()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ArrayID);
	}

}