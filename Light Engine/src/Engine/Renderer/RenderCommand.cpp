#include "ltpch.h"
#include "RenderCommand.h"

#include "Events/Event.h"

namespace Light {

	GraphicsContext* RenderCommand::s_GraphicsContext;

	void RenderCommand::SwapBuffers()
	{
		s_GraphicsContext->SwapBuffers();
	}

	void RenderCommand::HandleWindowEvents(Event& event)
	{
		s_GraphicsContext->HandleWindowEvents(event);
	}

}