#pragma once

#include "Core/Core.h"

#include "BufferLayout.h"
#include "Buffers.h"
#include "Shader.h"

namespace Light {

	class Renderer
	{
	private:
		static std::shared_ptr<Shader> s_Shader;
		static std::shared_ptr<VertexBuffer> s_Buffer;
		static std::shared_ptr<IndexBuffer> s_Indices;
		static std::shared_ptr<BufferLayout> s_Layout;

		static float* s_BufferMapStart;
		static float* s_BufferMapCurrent;
		static float* s_BufferMapEnd;

		static unsigned int s_QuadCount;
	public:
		static void Init();

		static void Start();
		static void DrawQuad(float x, float y, float width, float height);
		static void End();
	};

}