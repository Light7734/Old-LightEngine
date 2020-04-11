#pragma once

#include "Core/Core.h"

#include "Renderer/MSAA.h"

namespace Light {

	class glMSAA : public MSAA 
	{
	private:
		unsigned int m_BufferID;
		unsigned int m_TextureID;

		int m_Width, m_Height;
	public:
		glMSAA(unsigned int samples);
		~glMSAA();

		void BindFrameBuffer() override;

		void Resolve() override;

		void Resize(unsigned int width, unsigned int height) override;
	};

}