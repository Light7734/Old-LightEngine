#include "ltpch.h"
#include "Renderer.h"

#include "Camera.h"
#include "Framebuffer.h"
#include "MSAA.h"
#include "RenderCommand.h"
#include "Texture.h"

#include "Font/Font.h"
#include "Shaders/QuadShader.h"
#include "Shaders/TextShader.h"

#include "Core/Timer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Light {

	Renderer::QuadRenderer Renderer::s_QuadRenderer;
	Renderer::TextRenderer Renderer::s_TextRenderer;

	std::vector<std::shared_ptr<Framebuffer>> Renderer::s_Framebuffers;
	std::shared_ptr<VertexBuffer> Renderer::s_FramebufferVertices;
	std::shared_ptr<VertexLayout> Renderer::s_FramebufferLayout;

	std::shared_ptr<ConstantBuffer> Renderer::s_ViewProjBuffer;

	std::shared_ptr<MSAA> Renderer::s_MSAA;
	bool Renderer::s_MSAAEnabled = false;

	void Renderer::Init(unsigned int MSAASampleCount, bool MSAA)
	{
		LT_PROFILE_FUNC();

		// framebuffers
		s_Framebuffers.clear();

		float framebufferVertices[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
		};

		s_FramebufferVertices = VertexBuffer::Create(framebufferVertices, sizeof(float) * 4, 6);
		s_FramebufferLayout = VertexLayout::Create(nullptr, s_FramebufferVertices, { { "POSITION" , VertexElementType::Float2 },
																					 { "TEXCOORDS", VertexElementType::Float2 } });

		// view projection buffer
		s_ViewProjBuffer = ConstantBuffer::Create(ConstantBufferIndex_ViewProjection, sizeof(glm::mat4) * 2);

		// MSAA
		SetMSAA(MSAA);
		SetMSAASampleCount(MSAASampleCount);

		//=============== QUAD RENDERER ===============//
		s_QuadRenderer.shader = Shader::Create(QuadShaderSrc_VS, QuadShaderSrc_FS);

		s_QuadRenderer.vertexBuffer = VertexBuffer::Create(nullptr, sizeof(QuadRenderer::QuadVertexData), LT_MAX_BASIC_SPRITES * 4);
		s_QuadRenderer.indexBuffer = IndexBuffer::Create(nullptr, LT_MAX_BASIC_SPRITES * 6);

		s_QuadRenderer.vertexLayout = VertexLayout::Create(s_QuadRenderer.shader, s_QuadRenderer.vertexBuffer, s_QuadRenderer.shader->GetElements());
		//=============== QUAD RENDERER ===============//

		//================== TEXT RENDERER ==================//
		s_TextRenderer.shader = Shader::Create(TextShaderSrc_VS, TextShaderSrc_FS);

		s_TextRenderer.vertexBuffer = VertexBuffer::Create(nullptr, sizeof(TextRenderer::TextVertexData), LT_MAX_TEXT_SPRITES * 4);
		s_TextRenderer.indexBuffer = IndexBuffer::Create(nullptr, LT_MAX_TEXT_SPRITES * 6);
																																						  
		s_TextRenderer.vertexLayout = VertexLayout::Create(s_TextRenderer.shader, s_TextRenderer.vertexBuffer, s_QuadRenderer.shader->GetElements());
		//================== TEXT RENDERER ==================//
	}

	void Renderer::BeginFrame()
	{
		if (s_MSAAEnabled)
			s_MSAA->BindFrameBuffer();
		else if (!s_Framebuffers.empty())
			s_Framebuffers[0]->BindAsTarget();
	}

	void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		// set view projection buffer
		camera->CalculateViewProjection();

		glm::mat4* map = (glm::mat4*)s_ViewProjBuffer->Map();
		map[0] = camera->GetView();
		map[1] = camera->GetProjection();
		s_ViewProjBuffer->UnMap();

		// map renderer's vertex buffer
		s_QuadRenderer.Map();
		s_TextRenderer.Map();
	}
	
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, SubTexture* texture, const glm::vec4& tint)
	{
		if (s_QuadRenderer.mapCurrent == s_QuadRenderer.mapEnd)
		{
			LT_CORE_ERROR("Renderer::DrawQuad: calls to this function exceeded its limit: {}", s_QuadRenderer.GetMaximumQuadCount());
			EndScene();

			s_QuadRenderer.Map();
			s_TextRenderer.Map();
		}

		/* locals */
		const float xMin = position.x - size.x / 2.0f;
		const float xMax = position.x + size.x / 2.0f;
		const float yMin = position.y - size.y / 2.0f;
		const float yMax = position.y + size.y / 2.0f;

		// TOP_LEFT  [ -0.5, -0.5 ]
		s_QuadRenderer.mapCurrent->position = { xMin, yMin };
		s_QuadRenderer.mapCurrent->str = { texture->xMin, texture->yMin, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// TOP_RIGHT [ 0.5, -0.5 ]
		s_QuadRenderer.mapCurrent->position = { xMax, yMin };
		s_QuadRenderer.mapCurrent->str = { texture->xMax, texture->yMin, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// BOTTOM_RIGHT [ 0.5, 0.5 ]
		s_QuadRenderer.mapCurrent->position = { xMax, yMax };
		s_QuadRenderer.mapCurrent->str = { texture->xMax, texture->yMax, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// BOTTOM_LEFT [ -0.5, 0.5 ]
		s_QuadRenderer.mapCurrent->position = { xMin, yMax };
		s_QuadRenderer.mapCurrent->str = { texture->xMin, texture->yMax, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		s_QuadRenderer.quadCount++;
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, SubTexture* texture, const glm::vec4& tint)
	{
		if (s_QuadRenderer.mapCurrent == s_QuadRenderer.mapEnd)
		{
			LT_CORE_ERROR("Renderer::DrawQuad: calls to this function exceeded its limit: {}", s_QuadRenderer.GetMaximumQuadCount());
			EndScene();

			s_QuadRenderer.Map();
			s_TextRenderer.Map();
		}

		/* locals */
		const float COS = std::cos(angle);
		const float SIN = std::sin(angle);

		glm::vec2 quadCos = COS * size / 2.0f;
		glm::vec2 quadSin = SIN * size / 2.0f;

		/* write to the buffer */
		// TOP_LEFT  [ -0.5, -0.5 ]
		s_QuadRenderer.mapCurrent->position = glm::vec2(-(quadCos.x - quadSin.y), -(quadSin.x + quadCos.y)) + position;
		s_QuadRenderer.mapCurrent->str = { texture->xMin, texture->yMin, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// TOP_RIGHT [ 0.5, -0.5 ]
		s_QuadRenderer.mapCurrent->position = glm::vec2(quadCos.x - -quadSin.y, quadSin.x + -quadCos.y) + position;
		s_QuadRenderer.mapCurrent->str = glm::vec3(texture->xMax, texture->yMin, texture->sliceIndex);
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// BOTTOM_RIGHT [ 0.5, 0.5 ]
		s_QuadRenderer.mapCurrent->position = glm::vec2(quadCos.x - quadSin.y, quadSin.x + quadCos.y) + position;
		s_QuadRenderer.mapCurrent->str = { texture->xMax, texture->yMax, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		// BOTTOM_LEFT [ -0.5, 0.5 ]
		s_QuadRenderer.mapCurrent->position = glm::vec2(-quadCos.x - quadSin.y, -quadSin.x + quadCos.y) + position;
		s_QuadRenderer.mapCurrent->str = { texture->xMin, texture->yMax, texture->sliceIndex };
		s_QuadRenderer.mapCurrent->tint = tint;
		s_QuadRenderer.mapCurrent++;

		s_QuadRenderer.quadCount++;
	}

	void Renderer::DrawString(const std::string& text, const std::shared_ptr<Font>& font,
	                          const glm::vec2& position, float scale, const glm::vec4& tint)
	{
		/* locals */
		glm::vec2 beginning(position);
		float advance = 0.0f;

		// calculate beginning
		for (const auto& ch : text)
			beginning.x -= (font->GetCharacterData(ch).advance) * scale / 2.0f;

		for (const auto& ch : text)
		{
			if (s_TextRenderer.mapCurrent == s_TextRenderer.mapEnd)
			{
				LT_CORE_ERROR("Renderer::DrawString: calls to this function exceeded its limit (or string too long): {}", s_TextRenderer.GetMaximumQuadCount());
				EndScene();

				s_QuadRenderer.Map();
				s_TextRenderer.Map();
			}

			/* locals */
			const FontCharData character = font->GetCharacterData(ch);

			const float xMin = beginning.x + (character.bearing.x * scale) + advance;
			const float yMin = beginning.y - (character.bearing.y) * scale;

			const float xMax = xMin + character.size.x * scale;
			const float yMax = yMin + character.size.y * scale;

			advance += (character.advance) * scale;

			/* write to the buffer */
			// TOP_LEFT [ 0.0, 0.0 ]
			s_TextRenderer.mapCurrent->position = { xMin, yMin };
			s_TextRenderer.mapCurrent->str = { character.glyph.xMin, character.glyph.yMin, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// TOP_RIGHT [ 1.0, 0.0 ]
			s_TextRenderer.mapCurrent->position = { xMax, yMin };
			s_TextRenderer.mapCurrent->str = { character.glyph.xMax, character.glyph.yMin, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// BOTTOM_RIGHT [ 1.0, 1.0 ]
			s_TextRenderer.mapCurrent->position = { xMax, yMax };
			s_TextRenderer.mapCurrent->str = { character.glyph.xMax, character.glyph.yMax, character.glyph.sliceIndex};
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// BOTTOM_LEFT [ 0.0, 1.0 ] 
			s_TextRenderer.mapCurrent->position = { xMin, yMax };
			s_TextRenderer.mapCurrent->str = { character.glyph.xMin, character.glyph.yMax, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			s_TextRenderer.quadCount++;
		}
	}

	void Renderer::DrawString(const std::string& text, const std::shared_ptr<Font>& font,
	                          const glm::vec2& position, float angle, float scale, const glm::vec4& tint)
	{
		/* locals */
		glm::vec2 advance(0.0f);
		glm::vec2 beginning(position);

		const float COS = std::cos(angle) * scale;
		const float SIN = std::sin(angle) * scale;

		// calculate beginning
		for (const auto& ch : text)
		{
			beginning.x -= ((font->GetCharacterData(ch).advance) * COS) / 2.0f;
			beginning.y -= ((font->GetCharacterData(ch).advance) * SIN) / 2.0f;
		}

		for (const auto& ch : text)
		{
			if (s_TextRenderer.mapCurrent == s_TextRenderer.mapEnd)
			{
				LT_CORE_ERROR("Renderer::DrawString: calls to this function exceeded its limit (or string too long): {}", s_TextRenderer.GetMaximumQuadCount());
				EndScene();

				s_QuadRenderer.Map();
				s_TextRenderer.Map();
			}

			/* locals */
			const auto& character = font->GetCharacterData(ch);
			 
			glm::vec2 bearing(character.bearing.x  * COS + (character.bearing.y) * SIN,
			                  character.bearing.x  * SIN - (character.bearing.y) * COS);

			glm::vec2 charPosition = beginning + advance + bearing;

			advance.x += (character.advance) * COS;
			advance.y += (character.advance) * SIN;

			const glm::vec2 charCos = COS * character.size;
			const glm::vec2 charSin = SIN * character.size;

			/* write to the buffer  */	
			// TOP_LEFT [ 0.0, 0.0 ]
			s_TextRenderer.mapCurrent->position = charPosition;
			s_TextRenderer.mapCurrent->str = { character.glyph.xMin, character.glyph.yMin, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// TOP_RIGHT [ 1.0, 0.0 ]
			s_TextRenderer.mapCurrent->position = glm::vec2(charCos.x, charSin.x) + charPosition;
			s_TextRenderer.mapCurrent->str = { character.glyph.xMax, character.glyph.yMin, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// BOTTOM_RIGHT [ 1.0, 1.0 ]
			s_TextRenderer.mapCurrent->position = glm::vec2(charCos.x - charSin.y, charSin.x + charCos.y) + charPosition;
			s_TextRenderer.mapCurrent->str = { character.glyph.xMax, character.glyph.yMax, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			// BOTTOM_LEFT [ 0.0, 1.0 ] 
			s_TextRenderer.mapCurrent->position = glm::vec2(-charSin.y, charCos.y) + charPosition;
			s_TextRenderer.mapCurrent->str = { character.glyph.xMin, character.glyph.yMax, character.glyph.sliceIndex };
			s_TextRenderer.mapCurrent->tint = tint;
			s_TextRenderer.mapCurrent++;

			s_TextRenderer.quadCount++;
		}
	}

	void Renderer::EndScene()
	{
		s_TextRenderer.vertexBuffer->UnMap();
		s_QuadRenderer.vertexBuffer->UnMap();

		//=============== QUAD RENDERER ===============//
		if (s_QuadRenderer.quadCount)
		{
			s_QuadRenderer.Bind();
				
			RenderCommand::DrawIndexed(s_QuadRenderer.quadCount * 6);
			s_QuadRenderer.quadCount = 0;
		}
		//=============== QUAD RENDERER ===============//

		//================== TEXT RENDERER ==================//
		if (s_TextRenderer.quadCount)
		{
			s_TextRenderer.Bind();

			RenderCommand::DrawIndexed(s_TextRenderer.quadCount * 6);
			s_TextRenderer.quadCount = 0;
		}
		//================== TEXT RENDERER ==================//
	}

	void Renderer::EndFrame()
	{
		if (!s_Framebuffers.empty())
		{
			if (s_MSAAEnabled)
			{
				s_Framebuffers[0]->BindAsTarget();
				s_MSAA->Resolve();
			}

			s_FramebufferVertices->Bind();
			s_FramebufferLayout->Bind();

			for (int i = 1; i < s_Framebuffers.size(); i++)
			{
				s_Framebuffers[i]->BindAsTarget();
				s_Framebuffers[i - 1]->BindAsResource();
				RenderCommand::Draw(6);
			}

			RenderCommand::DefaultRenderBuffer();
			s_Framebuffers.back()->BindAsResource();
			RenderCommand::Draw(6);
		}
		else
		{
			if (s_MSAAEnabled)
			{
				RenderCommand::DefaultRenderBuffer();
				s_MSAA->Resolve();
			}
		}
	}

	void Renderer::AddFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		auto it = std::find(s_Framebuffers.begin(), s_Framebuffers.end(), framebuffer);

		if (it == s_Framebuffers.end())
			s_Framebuffers.push_back(framebuffer);
		else
			LT_CORE_ERROR("Renderer::AddFramebuffer: cannot add the same framebuffer twice");
	}

	void Renderer::RemoveFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		auto it = std::find(s_Framebuffers.begin(), s_Framebuffers.end(), framebuffer);

		if (it != s_Framebuffers.end())
			s_Framebuffers.erase(it);
		else
			LT_CORE_ERROR("Renderer::RemoveFramebuffer: failed to find framebuffer");
	}

	void Renderer::SetMSAA(bool enabled)
	{
		s_MSAAEnabled = enabled;
	}

	void Renderer::SetMSAASampleCount(unsigned int sampleCount)
	{
		LT_CORE_ASSERT((sampleCount & (sampleCount - 1)) == 0, "Renderer::SetMSAASampleCount: sampleCount '{}' is not power of 2", sampleCount);
		LT_CORE_ASSERT(sampleCount <= 16, "Renderer::SetMSAASampleCount: sampleCount too high: '{}', maximum sampleCount should be 16", sampleCount);
		LT_CORE_ASSERT(sampleCount != 0, "Renderer::SetMSAASampleCount: sampleCount cannot be 0");

		s_MSAA = MSAA::Create(sampleCount);
	}

} 