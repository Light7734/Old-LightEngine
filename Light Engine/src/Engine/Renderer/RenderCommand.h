#pragma once

#include "Core/Core.h"

namespace Light {

	class GraphicsContext;

	class RenderCommand
	{
	private:
		static GraphicsContext* s_GraphicsContextRef;
		static float s_BackBufferClearColor[4];
	public:
		static bool HasContext() { return s_GraphicsContextRef; }

		static void SwapBuffers();

		static void ClearBackbuffer();

		static void Draw(unsigned int count);
		static void DrawIndexed(unsigned int count);

		static void DefaultRenderBuffer();
	private:
		friend class GraphicsContext;
		static inline void SetGraphicsContext(GraphicsContext* context) { s_GraphicsContextRef = context; }
	};

}