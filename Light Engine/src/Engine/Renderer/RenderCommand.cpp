#include "ltpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Light {

	GraphicsContext* RenderCommand::s_pToGraphicsContext = nullptr;

	float RenderCommand::s_backBuffercolor[4] = { 0.0f, 0.0f, 0.0f, 0.0f};

	void RenderCommand::SwapBuffers()
	{
		s_pToGraphicsContext->SwapBuffers();
	}

	void RenderCommand::ClearBackbuffer()
	{
		s_pToGraphicsContext->ClearBackbuffer(s_backBuffercolor);
	}

	void RenderCommand::SetClearBackbufferColor(float colors[4])
	{
		s_backBuffercolor[0] = colors[0];
		s_backBuffercolor[1] = colors[1];
		s_backBuffercolor[2] = colors[2];
		s_backBuffercolor[3] = colors[3];
	}

	void RenderCommand::Draw(unsigned int count)
	{
		s_pToGraphicsContext->Draw(count);
	}

	void RenderCommand::DrawIndexed(unsigned int count)
	{
		s_pToGraphicsContext->DrawIndexed(count);
	}

}