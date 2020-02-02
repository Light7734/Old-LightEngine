#include "ltpch.h"
#include "glTexture.h"

#include "Utility/FileManager.h"

#include <glad/glad.h>

namespace Light {

	glTexture::glTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels)
	{
		unsigned int format =
			channels == 4 ? GL_RGBA :
			channels == 3 ? GL_RGB  :
			channels == 2 ? GL_RG   :
			channels == 1 ? GL_RED  : GL_NONE;
		
		unsigned int internalFormat =
			format == GL_RGBA ? GL_RGBA8 :
			format == GL_RGB  ? GL_RGB8  :
			format == GL_RG   ? GL_RG8   :
			format == GL_RED  ? GL_R8    : GL_NONE;


		LT_CORE_ASSERT(data, "Texture data is null");
		LT_CORE_ASSERT(width && height, "Width or height cannot be 0, [{}, {}]", width, height);
		LT_CORE_ASSERT(format, "Invalid texture format: {}, channels: {}", format, channels);
		LT_CORE_ASSERT(internalFormat, "Invalid texture internalFormat: {}, channels: {}", internalFormat, channels);

		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,  width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_TextureID);
	}

	glTexture::~glTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void glTexture::Bind(unsigned int index/* = 0 */)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		m_BoundSlot = index;
		b_Bound = true;
	}

}