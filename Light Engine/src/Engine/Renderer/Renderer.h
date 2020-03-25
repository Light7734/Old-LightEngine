#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class Shader;
	class VertexLayout;
	class ConstantBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Framebuffer;

	class Camera;

	struct TextureCoordinates;

	class Renderer
	{
	private:
		//=============== BASIC QUAD RENDERER ===============//
		struct BasicQuadRenderer
		{
			std::shared_ptr<Shader>       shader;
			std::shared_ptr<VertexLayout> vertexLayout;
			std::shared_ptr<IndexBuffer>  indexBuffer;
			std::shared_ptr<VertexBuffer> vertexBuffer;

			float* mapCurrent = nullptr;
			float* mapEnd     = nullptr;

			unsigned int quadCount = 0;
		};
		static BasicQuadRenderer s_QuadRenderer;
		//=============== BASIC QUAD RENDERER ===============//

		// camera
		static std::shared_ptr<Camera> s_Camera;
		static std::shared_ptr<ConstantBuffer> s_ViewProjBuffer;

		// framebuffers
		static std::vector<std::shared_ptr<Framebuffer>> s_Framebuffers;
		static std::shared_ptr<VertexBuffer> s_FramebufferVertices;
		static std::shared_ptr<VertexLayout> s_FramebufferLayout;
	public:
		static void Begin();

		// basic quad
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const TextureCoordinates* uv, const glm::vec4& tint);

		static void End();

		// frame buffers
		static void AddFramebuffer(std::shared_ptr<Framebuffer> framebuffer);
		static void RemoveFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

		// camera
		static inline void SetCamera(std::shared_ptr<Camera> camera) { s_Camera = camera; }
	private:
		friend class GraphicsContext;
		static void Init();
	};

}