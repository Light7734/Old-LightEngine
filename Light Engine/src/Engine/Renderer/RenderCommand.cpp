#include "ltpch.h"
#include "RenderCommand.h"

#include "Events/Event.h"

namespace Light {

	std::unique_ptr<GraphicsContext> RenderCommand::s_GraphicsContext;

	void RenderCommand::SwapBuffers()
	{
		s_GraphicsContext->SwapBuffers();
	}

	void RenderCommand::EnableVSync()
	{
		s_GraphicsContext->EnableVSync();
	}


	void RenderCommand::DisableVSync()
	{
		s_GraphicsContext->DisableVSync();
	}

	void RenderCommand::Clear()
	{
		s_GraphicsContext->Clear();
	}

	void RenderCommand::ClearBuffer(float r, float g, float b, float a)
	{
		s_GraphicsContext->ClearBuffer(r, g, b, a);
	}

	void RenderCommand::HandleWindowEvents(Event& event)
	{
		s_GraphicsContext->HandleWindowEvents(event);
	}

}