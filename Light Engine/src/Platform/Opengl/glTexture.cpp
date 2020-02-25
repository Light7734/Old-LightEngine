#include "ltpch.h"
#include "glTexture.h"

#include <glad/glad.h>

namespace Light {

	unsigned int glTextureAtlas::m_AtlasID = 0;

	glTextureAtlas::glTextureAtlas(const TextureData& data)
	{
		if (!s_Width && !s_Height)
			Init(data.width, data.height);

		LT_CORE_ASSERT(data.width == s_Width && data.height == s_Height,
		               "glTextureAtlas::glTextureAtlas: Texture slice dimensions do not match the array: slice[{}, {}] != array[{}, {}] ",
		               data.width, data.height, s_Width, s_Height);

		glTextureSubImage3D(m_AtlasID, 0, 0, 0, m_Index, data.width, data.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data.pixels);
		glGenerateTextureMipmap(m_AtlasID);
	}

	void glTextureAtlas::Init(unsigned int width, unsigned int height)
	{
		s_Width = width;
		s_Height = height;

		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_AtlasID);
		glTextureStorage3D(m_AtlasID, std::log(std::max(width, height)) + 1, GL_RGBA8, width, height, 16);

		glTextureParameteri(m_AtlasID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_AtlasID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_AtlasID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_AtlasID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_AtlasID);
	}

	void glTextureAtlas::DestroyTextureArray()
	{
		glDeleteTextures(1, &m_AtlasID);

		s_Width = 0;
		s_Height = 0;
	}

}