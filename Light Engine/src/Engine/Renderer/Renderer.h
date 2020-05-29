#pragma once

#include "Buffers.h"
#include "Shader.h"
#include "VertexLayout.h"

#include "Core/Core.h"

#include <glm/glm.hpp>

#define LT_MAX_BASIC_SPRITES    10000
#define LT_MAX_TEXT_SPRITES     2000

namespace Light {

	class Framebuffer;
	class MSAA;

	class Font;

	class Camera;

	struct TextureCoordinates;

	struct RendererProgram
	{
		virtual void Reset() = 0;
		virtual void Bind() = 0;
		virtual void Map() = 0;

		virtual inline unsigned int GetMaximumQuadCount() = 0;
	};
		
	class Renderer
	{
	private:
		//=============== QUAD RENDERER ===============//
		struct QuadRenderer : public RendererProgram
		{
			std::shared_ptr<Shader>       shader;
			std::shared_ptr<VertexLayout> vertexLayout;
			std::shared_ptr<VertexBuffer> vertexBuffer;
			std::shared_ptr<IndexBuffer>  indexBuffer;

			struct QuadVertexData
			{
				glm::vec2 position;
				glm::vec3 str;
				glm::vec4 tint;
			};

			QuadVertexData* mapCurrent = nullptr;
			QuadVertexData* mapEnd     = nullptr;

			unsigned int quadCount = 0;

			void Reset() override
			{
				shader.reset();
				vertexLayout.reset();
				vertexBuffer.reset();
				indexBuffer.reset();
			}

			void Map() override
			{
				mapCurrent = (QuadVertexData*)vertexBuffer->Map();
				mapEnd = mapCurrent + LT_MAX_BASIC_SPRITES * 4;
			}

			void Bind() override
			{
				shader->Bind();
				vertexLayout->Bind();
				vertexBuffer->Bind();
				indexBuffer->Bind();
			}

			inline unsigned int GetMaximumQuadCount() override { return LT_MAX_BASIC_SPRITES; }
		};
		//=============== QUAD RENDERER ===============//

		//=============== TEXT RENDERER ===============//
		struct TextRenderer : public RendererProgram
		{
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexLayout> vertexLayout;
			std::shared_ptr<VertexBuffer> vertexBuffer;
			std::shared_ptr<IndexBuffer> indexBuffer;

			struct TextVertexData
			{
				glm::vec2 position;
				glm::vec3 str;
				glm::vec4 tint;
			};

			TextVertexData* mapCurrent = nullptr;
			TextVertexData* mapEnd = nullptr;

			unsigned int quadCount = 0;

			void Reset() override
			{
				shader.reset();
				vertexLayout.reset();
				vertexBuffer.reset();
				indexBuffer.reset();
			}

			void Map() override
			{
				mapCurrent = (TextVertexData*)vertexBuffer->Map();
				mapEnd = mapCurrent + LT_MAX_TEXT_SPRITES * 4;
			}
			
			void Bind() override
			{
				shader->Bind();
				vertexLayout->Bind();
				vertexBuffer->Bind();
				indexBuffer->Bind();
			}

			inline unsigned int GetMaximumQuadCount() override { return LT_MAX_TEXT_SPRITES; }
		};
		//=============== TEXT RENDERER ===============//


		// renderer programs
		static QuadRenderer s_QuadRenderer;
		static TextRenderer s_TextRenderer;

		// camera
		static std::shared_ptr<ConstantBuffer> s_ViewProjBuffer;

		// framebuffers
		static std::vector<std::shared_ptr<Framebuffer>> s_Framebuffers;
		static std::shared_ptr<VertexBuffer> s_FramebufferVertices;
		static std::shared_ptr<VertexLayout> s_FramebufferLayout;

		// MSAA
		static std::shared_ptr<MSAA> s_MSAA;
		static bool s_MSAAEnabled;
	public:
		static void BeginFrame();
		static void BeginScene(const std::shared_ptr<Camera>& camera);

		// quad renderer
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const TextureCoordinates& texture, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, TextureCoordinates* texture, const glm::vec4& tint = glm::vec4(1.0f));

		// text renderer
		static void DrawString(const std::string& text, const std::shared_ptr<Font>& font,
		                       const glm::vec2& position, float scale = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		
		static void DrawString(const std::string& text, const std::shared_ptr<Font>& font,
		                       const glm::vec2& position, float angle, float scale = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

		static void EndScene();
		static void EndFrame();

		// frame buffers
		static void AddFramebuffer(std::shared_ptr<Framebuffer> framebuffer);
		static void RemoveFramebuffer(std::shared_ptr<Framebuffer> framebuffer);
	private:
		friend class GraphicsContext;
		static void Init(unsigned int MSAASampleCount, bool MSAA);
		static void Terminate();

		static void SetMSAA(bool enabled);
		static void SetMSAASampleCount(unsigned int sampleCount);
	};

}