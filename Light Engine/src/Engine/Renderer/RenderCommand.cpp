#include "ltpch.h"
#include "RenderCommand.h"

#include "Events/Event.h"

namespace Light {

	GraphicsContext* RenderCommand::s_GraphicsContext;

	void RenderCommand::SwapBuffers()
	{
		s_GraphicsContext->SwapBuffers();
	}

	void RenderCommand::ClearBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		s_GraphicsContext->ClearBuffer(r, g, b, a);
	}

	void RenderCommand::HandleWindowEvents(Event& event)
	{
		s_GraphicsContext->HandleWindowEvents(event);
	}

}