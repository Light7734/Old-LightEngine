#include "ltpch.h"
#include "glMSAA.h"

#include "Renderer/GraphicsContext.h"

#include <glad/glad.h>

namespace Light {

	glMSAA::glMSAA(unsigned int samples)
		: m_Width(GraphicsContext::GetResolution().width), m_Height(GraphicsContext::GetResolution().height)
	{
		LT_PROFILE_FUNC();

		// create framebuffer and color texture
		glCreateFramebuffers(1, &m_BufferID);
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_TextureID);

		// bind framebuffer and color texture
		glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);


		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA,
		                        GraphicsContext::GetConfigurations().resolution.width,
		                        GraphicsContext::GetConfigurations().resolution.height,
		                        GL_TRUE);
			
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureID, 0);

		// check
		LT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		               "glMSAA::glMSAA: framebuffer status is not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glMSAA::~glMSAA()
	{
		LT_PROFILE_FUNC();

		glDeleteTextures(1, &m_TextureID);
		glDeleteFramebuffers(1, &m_BufferID);
	}

	void glMSAA::BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void glMSAA::Resize(unsigned int width, unsigned int height)
	{
		LT_PROFILE_FUNC();

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	void glMSAA::Resolve()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_BufferID);
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

}