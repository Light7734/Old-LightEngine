#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class Shader;
	class BufferLayout;
	class IndexBuffer;
	class VertexBuffer;

	class Camera;

	struct TextureCoordinates;

	class Renderer
	{
	private:
		struct BasicQuadRenderer
		{
			std::shared_ptr<Shader>       shader;
			std::shared_ptr<BufferLayout> bufferLayout;
			std::shared_ptr<IndexBuffer>  indexBuffer;
			std::shared_ptr<VertexBuffer> vertexBuffer;

			float* mapCurrent = nullptr;
			float* mapEnd     = nullptr;

			unsigned int quadCount = 0;
		};
		static BasicQuadRenderer s_QuadRenderer;
	public:
		static void Start(Camera& camera);

		//=============== BASIC QUAD RENDERER ===============//
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
		                     const TextureCoordinates* textureCoordinates, const glm::vec4& tint);
		//=============== BASIC QUAD RENDERER ===============//

		static void End();
	private:
		friend class GraphicsContext;
		static void Init();
	};

}