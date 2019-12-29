#pragma once

#include "GraphicsContext.h"

#include "Core/Core.h"

#include <memory>

namespace Light {

	class Event;

	class RenderCommand
	{
	private:
		static std::unique_ptr<GraphicsContext> s_GraphicsContext;
	public:
		static inline void SetGraphicsContext(std::unique_ptr<GraphicsContext> context) 
			{  s_GraphicsContext = std::move(context); }

		static bool isInitialized() { return s_GraphicsContext ? true : false; }

		static void SwapBuffers();

		static void EnableVSync ();
		static void DisableVSync();

		static void Clear();
		static void ClearBuffer(float r, float g, float b, float a);

		static void HandleWindowEvents(Event& event);
	};

}