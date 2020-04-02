#include "ltpch.h"
#include "glTexture.h"

#include "Utility/FileManager.h"	

#include <glad/glad.h>

namespace Light {

	glTextureArray::glTextureArray(unsigned int slices, unsigned int channels)
		: m_Channels(channels)
	{
		LT_PROFILE_FUNC();

		// set the format and validate it
		m_Format = 
			channels == 4 ? GL_RGBA :
			channels == 3 ? GL_RGB  :
			channels == 2 ? GL_RG   :
			channels == 1 ? GL_RED  :
			                GL_NONE ;

		LT_CORE_ASSERT(m_Format, "glTextureArray::glTextureArray: invalid number of channels");


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
		LT_PROFILE_FUNC();

		glDeleteTextures(1, &m_ArrayID);
	}

	void glTextureArray::CreateSliceWithAtlas(const std::string& texturePath, const std::string& atlasName, const std::string& atlasPath)
	{
		LT_PROFILE_FUNC();

		// load texture
		TextureData data;
		data.pixels = FileManager::LoadTextureFile(texturePath, &data.width, &data.height, &data.channels);
		
		// first slice? initialize width & height and specify the storage requirements
		if (!m_Width)
		{
			m_Width = data.width;
			m_Height = data.height;

			unsigned int internalFormat = 
				m_Format ==  GL_RGBA ? GL_RGBA8 :
				m_Format ==  GL_RGB  ? GL_RGB8  :
				m_Format ==  GL_RG   ? GL_RG8   :
				m_Format ==  GL_RED  ? GL_R8    :
				                       GL_NONE  ;

			glTextureStorage3D(m_ArrayID, std::log(std::max(m_Width, m_Height)) + 1, internalFormat, m_Width, m_Height, m_Depth);
		}


		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "glTextureArray::CreateAtlas: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "glTextureArray::CreateAtlas: failed to load texture file: {}", texturePath);

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "glTextureArray::CreateAtlas: texture dimensions do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);

		// write to texture array and generate mips
		UpdateSubTexture(0, 0, m_AvailableSlots.back(), m_Width, m_Height, data.pixels);
		GenerateMips();

		// create atlas
		m_Atlases[atlasName] = std::make_shared<TextureAtlas>(atlasName, atlasPath, m_AvailableSlots.back());
		m_AvailableSlots.pop_back();
	}

	unsigned int glTextureArray::CreateSlice(const std::string& texturePath)
	{
		LT_PROFILE_FUNC();

		// load texture
		TextureData data;
		data.pixels = FileManager::LoadTextureFile(texturePath, &data.width, &data.height, &data.channels);
		
		// first slice? initialize width & height and specify the storage requirements
		if (!m_Width)
		{
			m_Width = data.width;
			m_Height = data.height;

			unsigned int internalFormat = 
				m_Format ==  GL_RGBA ? GL_RGBA8 :
				m_Format ==  GL_RGB  ? GL_RGB8  :
				m_Format ==  GL_RG   ? GL_RG8   :
				m_Format ==  GL_RED  ? GL_R8    :
				                       GL_NONE  ;

			glTextureStorage3D(m_ArrayID, std::log(std::max(m_Width, m_Height)) + 1, internalFormat, m_Width, m_Height, m_Depth);
		}

		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "glTextureArray::CreateSlice: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "glTextureArray::CreateSlice: failed to load texture file: {}", texturePath)

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "glTextureArray::CreateSlice: texture dimensions do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);

		// store, then pop selected index
		unsigned int sliceIndex = m_AvailableSlots.back();
		m_AvailableSlots.pop_back();

		// write to texture array and generate mips
		UpdateSubTexture(0, 0, sliceIndex, m_Width, m_Height, data.pixels);
		GenerateMips();

		return sliceIndex;
	}

	unsigned int glTextureArray::CreateSlice(unsigned int width, unsigned int height, void* pixels)
	{
		LT_PROFILE_FUNC();

		// first slice? initialize width & height and specify the storage requirements
		if (!m_Width)
		{
			m_Width = width;
			m_Height = height;
			
			unsigned int internalFormat = 
				m_Format ==  GL_RGBA ? GL_RGBA8 :
				m_Format ==  GL_RGB  ? GL_RGB8  :
				m_Format ==  GL_RG   ? GL_RG8   :
				m_Format ==  GL_RED  ? GL_R8    :
				                       GL_NONE  ;

			LT_CORE_ASSERT(width && height, "glTextureArray::CreateSlice: invalid slice dimensions: [{}x{}]",
						   width, height);

			glTextureStorage3D(m_ArrayID, std::log(std::max(m_Width, m_Height)) + 1, internalFormat, m_Width, m_Height, m_Depth);
		}


		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "glTextureArray::CreateSlice: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(width == m_Width && height == m_Height,
		               "glTextureArray::CreateSlice: assigned dimensions do not match the array's: [{} x {}] != [{} x {}]",
		               width, height, m_Width, m_Height);


		// store, then pop selected index
		unsigned int sliceIndex = m_AvailableSlots.back();
		m_AvailableSlots.pop_back();


		// write to texture array and generate mips
		if (pixels)
		{
			UpdateSubTexture(0, 0, sliceIndex, m_Width, m_Height, pixels);
			GenerateMips();
		}

		return sliceIndex;
	}

	void glTextureArray::DeleteSliceWithAtlas(const std::string& atlasName)
	{
		if (m_Atlases[atlasName])
			m_AvailableSlots.push_back(m_Atlases[atlasName]->GetSliceIndex());
		else
			LT_CORE_ERROR("glTextureArray::DeleteAtlas: failed to find texture atlas: {}", atlasName);

		m_Atlases.erase(atlasName);
	}

	void glTextureArray::DeleteSlice(unsigned int sliceIndex)
	{
		m_AvailableSlots.push_back(sliceIndex);
	}

	void glTextureArray::UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels)
	{
		LT_PROFILE_FUNC();

		glPixelStorei(GL_UNPACK_ALIGNMENT, m_Channels);
		glTextureSubImage3D(m_ArrayID, 0, xoffset, yoffset, zoffset, width, height, 1, m_Format, GL_UNSIGNED_BYTE, pixels);
	}

	void glTextureArray::GenerateMips()
	{
		LT_PROFILE_FUNC();
		glGenerateTextureMipmap(m_ArrayID);
	}

	void glTextureArray::Bind()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ArrayID);
	}

}