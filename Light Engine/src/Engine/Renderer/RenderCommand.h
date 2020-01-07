#pragma once

#include "Core/Core.h"

#include <memory>

namespace Light {

	class GraphicsContext;

	class Event;

	class RenderCommand
	{
	private:
		static std::unique_ptr<GraphicsContext> s_GraphicsContext;
	public:
		static void SetGraphicsContext(std::unique_ptr<GraphicsContext> context);

		static bool isInitialized() { return s_GraphicsContext ? true : false; }

		static void HandleWindowEvents(Event& event);

		static void EnableVSync ();
		static void DisableVSync();

		static void SwapBuffers();

		static void Clear();
		static void ClearBuffer(float r, float g, float b, float a);

		static void Draw(unsigned int count);
		static void DrawIndexed(unsigned int count);
	};

}