#include "ltpch.h"
#include "RenderCommand.h"

#include "GraphicsContext.h"

namespace Light {

	GraphicsContext* RenderCommand::s_GraphicsContextRef = nullptr;

	float RenderCommand::s_BackBufferClearColor[4] = { 0.0f, 0.0f, 0.0f , 0.0f };

	void RenderCommand::SwapBuffers()
	{
		s_GraphicsContextRef->SwapBuffers();
	}

	void RenderCommand::ClearBackbuffer()
	{
		s_GraphicsContextRef->ClearBackbuffer(s_BackBufferClearColor);
	}

	void RenderCommand::Draw(unsigned int count)
	{
		s_GraphicsContextRef->Draw(count);
	}

	void RenderCommand::DrawIndexed(unsigned int count)
	{
		s_GraphicsContextRef->DrawIndexed(count);
	}

	void RenderCommand::DefaultRenderBuffer()
	{
		s_GraphicsContextRef->DefaultRenderBuffer();
	}

}