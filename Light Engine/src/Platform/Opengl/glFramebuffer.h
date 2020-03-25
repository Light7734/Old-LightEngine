#pragma once

#include "glVertexLayout.h"
#include "Renderer/Buffers.h"

#include "Core/Core.h"

#include "Renderer/Framebuffer.h"

namespace Light {

	class Shader;

	class glFramebuffer : public Framebuffer
	{
	private:
		std::shared_ptr<Shader> m_Shader;

		unsigned int m_FrameBufferID;
		unsigned int m_ColorTextureID;
	public:
		glFramebuffer(const std::string& vertex, const std::string& fragment);
		~glFramebuffer();

		void BindAsTarget() override;
		void BindAsResource() override;

		void Resize(unsigned int width, unsigned int height) override;
	};

}