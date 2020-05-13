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
	class MSAA;

	class Font;

	class Camera;

	struct TextureCoordinates;

	class Renderer
	{
	private:
		//=============== QUAD RENDERER ===============//
		struct QuadRenderer
		{
			std::shared_ptr<Shader>       shader;
			std::shared_ptr<VertexLayout> vertexLayout;
			std::shared_ptr<IndexBuffer>  indexBuffer;
			std::shared_ptr<VertexBuffer> vertexBuffer;

			struct BasicQuadVertexData
			{
				glm::vec2 position;
				glm::vec3 str;
				glm::vec4 tint;
			};

			BasicQuadVertexData* mapCurrent = nullptr;
			BasicQuadVertexData* mapEnd     = nullptr;

			unsigned int quadCount = 0;
		};
		static QuadRenderer s_QuadRenderer;
		//=============== QUAD RENDERER ===============//

		//=============== TEXT RENDERER ===============//
		struct TextRenderer
		{
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexLayout> vertexLayout;
			std::shared_ptr<IndexBuffer> indexBuffer;
			std::shared_ptr<VertexBuffer> vertexBuffer;

			struct TextVertexData
			{
				glm::vec2 position;
				glm::vec3 str;
				glm::vec4 tint;
			};

			TextVertexData* mapCurrent = nullptr;
			TextVertexData* mapEnd = nullptr;

			unsigned int quadCount = 0;
		};
		static TextRenderer s_TextRenderer;
		//=============== TEXT RENDERER ===============//

		// camera
		static std::shared_ptr<ConstantBuffer> s_ViewProjBuffer;
		static std::shared_ptr<Camera> s_Camera;

		// framebuffers
		static std::vector<std::shared_ptr<Framebuffer>> s_Framebuffers;
		static std::shared_ptr<VertexBuffer> s_FramebufferVertices;
		static std::shared_ptr<VertexLayout> s_FramebufferLayout;

		// MSAA
		static std::shared_ptr<MSAA> s_MSAA;
		static bool s_MSAAEnabled;
	public:
		static void BeginFrame();
		static void BeginLayer();

		// quad renderer
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
		                     TextureCoordinates* coords, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle,
		                            TextureCoordinates* coords, const glm::vec4& tint = glm::vec4(1.0f));

		// text renderer
		static void DrawString(const std::string& text, const std::shared_ptr<Font>& font,
		                       const glm::vec2& position, float scale = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		
		static void DrawString(const std::string& text, const std::shared_ptr<Font>& font,
		                       const glm::vec2& position, float angle, float scale = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

		static void EndLayer();
		static void EndFrame();

		// frame buffers
		static void AddFramebuffer(std::shared_ptr<Framebuffer> framebuffer);
		static void RemoveFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

		// camera
		static inline void SetCamera(std::shared_ptr<Camera> camera) { s_Camera = camera; }
	private:
		friend class GraphicsContext;
		static void Init(unsigned int MSAASampleCount, bool MSAA);

		static void SetMSAA(bool enabled);
		static void SetMSAASampleCount(unsigned int sampleCount);
	};

}