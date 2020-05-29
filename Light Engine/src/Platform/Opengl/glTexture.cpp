#include "ltpch.h"
#include "glTexture.h"

#include "Utility/FileManager.h"

#include <glad/glad.h>

namespace Light {

	glTextureArray::glTextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels)
		: TextureArray(width, height, depth, channels)
	{
		LT_PROFILE_FUNC();

		m_Format =  channels == 4 ? GL_RGBA : 
			        channels == 2 ? GL_RG   :
			        channels == 1 ? GL_RED  :
			                        GL_NONE ;

		unsigned int internalFormat = m_Format == GL_RGBA ? GL_RGBA8 :
		                              m_Format == GL_RG   ? GL_RG8   :
		                              m_Format == GL_RED  ? GL_R8    :
		                                                    GL_NONE  ;

		LT_CORE_ASSERT(m_Format, "glTextureArray::glTextureArray: invalid number of channels: {}", channels);

		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_TextureArrayID);

		glTextureParameteri(m_TextureArrayID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_TextureArrayID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureArrayID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_TextureArrayID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage3D(m_TextureArrayID, std::log(std::max(width, height)) + 1, internalFormat, width, height, depth);
	}

	glTextureArray::~glTextureArray()
	{
		LT_PROFILE_FUNC();

		glDeleteTextures(1, &m_TextureArrayID);
	}

	void glTextureArray::UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels)
	{
		LT_PROFILE_FUNC();

		glPixelStorei(GL_UNPACK_ALIGNMENT, m_Channels);
		glTextureSubImage3D(m_TextureArrayID, 0, xoffset, yoffset, zoffset, width, height, 1, m_Format, GL_UNSIGNED_BYTE, pixels);
	}

	void glTextureArray::UpdateSubTexture(const TextureCoordinates& uv, void* pixels)
	{
		LT_PROFILE_FUNC();

		glPixelStorei(GL_UNPACK_ALIGNMENT, m_Channels);
		glTextureSubImage3D(m_TextureArrayID, 0, uv.xMin, uv.yMin, uv.sliceIndex,
		                                         uv.xMax - uv.xMin, uv.yMax - uv.yMin, 1, m_Format, GL_UNSIGNED_BYTE, pixels);
	}

	void glTextureArray::GenerateMips()
	{
		LT_PROFILE_FUNC();
		glGenerateTextureMipmap(m_TextureArrayID);
	}

	void glTextureArray::Bind(unsigned int slot /* = 0 */) 
	{
		LT_PROFILE_FUNC();

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureArrayID);
	}
}