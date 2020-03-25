#pragma once

#include "GraphicsContext.h"

#include "Core/Core.h"

namespace Light {

	class RenderCommand
	{
	private:
		static GraphicsContext* s_pToGraphicsContext;
		static float s_BackBufferClearColor[4];
	public:
		static bool HasContext() { return s_pToGraphicsContext; }

		static void SwapBuffers();

		static void ClearBackbuffer();

		static void Draw(unsigned int count);
		static void DrawIndexed(unsigned int count);

		static void DefaultRenderBuffer();
	private:
		friend class GraphicsContext;
		static inline void SetGraphicsContext(GraphicsContext* context) { s_pToGraphicsContext = context; }
	};

}