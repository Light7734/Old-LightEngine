#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class BufferLayout;

	class Renderer
	{
	private:
		static std::shared_ptr<Shader> s_Shader;
		static std::shared_ptr<BufferLayout> s_Layout;
		static std::shared_ptr<IndexBuffer> s_Indices;
		static std::shared_ptr<VertexBuffer> s_VertexBuffer;

		static float* s_BufferMapCurrent;
		static float* s_BufferMapEnd;

		static unsigned int s_QuadCount;
	public:
		static void Init();

		static void Start();
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void End();
	};

}