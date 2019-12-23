#pragma once

#include "Core/Core.h"

#include "GraphicsContext.h"

namespace Light {

	class Event;

	class RenderCommand
	{
	private:
		static GraphicsContext* s_GraphicsContext;
	public:
		static inline void SetGraphicsContext(GraphicsContext* context) 
			{ s_GraphicsContext = context; }

		static void SwapBuffers();
		static void HandleWindowEvents(Event& event);
	};

}