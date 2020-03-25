#include "ltpch.h"
#include "RenderCommand.h"

namespace Light {

	GraphicsContext* RenderCommand::s_pToGraphicsContext = nullptr;

	float RenderCommand::s_BackBufferClearColor[4] = { 0.0f, 0.0f, 0.0f , 0.0f };

	void RenderCommand::SwapBuffers()
	{
		s_pToGraphicsContext->SwapBuffers();
	}

	void RenderCommand::ClearBackbuffer()
	{
		s_pToGraphicsContext->ClearBackbuffer(s_BackBufferClearColor);
	}

	void RenderCommand::Draw(unsigned int count)
	{
		s_pToGraphicsContext->Draw(count);
	}

	void RenderCommand::DrawIndexed(unsigned int count)
	{
		s_pToGraphicsContext->DrawIndexed(count);
	}

	void RenderCommand::DefaultRenderBuffer()
	{
		s_pToGraphicsContext->DefaultRenderBuffer();
	}

}