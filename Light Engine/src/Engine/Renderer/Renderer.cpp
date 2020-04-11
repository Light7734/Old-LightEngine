#include "ltpch.h"
#include "Renderer.h"

#include "Buffers.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "MSAA.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexLayout.h"

#include "Font/Font.h"
#include "Font/FontManager.h"

#include "Shaders/QuadShader.h"
#include "Shaders/TextShader.h"

#define LT_MAX_BASIC_SPRITES    10000
#define LT_MAX_TEXT_SPRITES     2000

namespace Light {

	Renderer::BasicQuadRenderer Renderer::s_QuadRenderer;
	Renderer::TextRenderer Renderer::s_TextRenderer;

	std::vector<std::shared_ptr<Framebuffer>> Renderer::s_Framebuffers;
	std::shared_ptr<VertexBuffer> Renderer::s_FramebufferVertices;
	std::shared_ptr<VertexLayout> Renderer::s_FramebufferLayout;

	std::shared_ptr<ConstantBuffer> Renderer::s_ViewProjBuffer;
	std::shared_ptr<Camera> Renderer::s_Camera;

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

		s_FramebufferVertices = VertexBuffer::Create(framebufferVertices, sizeof(framebufferVertices), sizeof(float) * 4);
		s_FramebufferLayout = VertexLayout::Create(nullptr, s_FramebufferVertices, { {"POSITION" , VertexElementType::Float2 },
																					 {"TEXCOORDS", VertexElementType::Float2} });

		// view projection buffer
		s_ViewProjBuffer = ConstantBuffer::Create(ConstantBufferIndex_ViewProjection, sizeof(glm::mat4) * 2);

		// camera
		if (!s_Camera)
			s_Camera = std::make_shared<Camera>(glm::vec2(0.0f, 0.0f), GraphicsContext::GetResolution().aspectRatio, 1000.0f);

		// MSAA
		SetMSAA(MSAA);
		SetMSAASampleCount(MSAASampleCount);

		// renderers
		unsigned int* indices = nullptr;
		unsigned int offset = 0;

		//=============== BASIC QUAD RENDERER ===============//
		// indices
		indices = new unsigned int [LT_MAX_BASIC_SPRITES * 6];
		offset = 0;

		for (int i = 0; i < LT_MAX_BASIC_SPRITES * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		// create bindables
		s_QuadRenderer.shader = Shader::Create(QuadShaderSrc_VS, QuadShaderSrc_FS);
		s_QuadRenderer.vertexBuffer = VertexBuffer::Create(nullptr, LT_MAX_BASIC_SPRITES * sizeof(float) * 9 * 4, sizeof(float) * 9);

		s_QuadRenderer.vertexLayout = VertexLayout::Create(s_QuadRenderer.shader,
		                                                   s_QuadRenderer.vertexBuffer,
		                                                   { {"POSITION" , VertexElementType::Float2},
		                                                     {"COLOR"    , VertexElementType::Float4},
		                                                     {"TEXCOORDS", VertexElementType::Float3}, });

		s_QuadRenderer.indexBuffer = IndexBuffer::Create(indices, LT_MAX_BASIC_SPRITES * sizeof(unsigned int) * 6);

		// free memory
		delete[] indices;
		//=============== BASIC QUAD RENDERER ===============//

		//================== TEXT RENDERER ==================//
		// indices
		indices = new unsigned int [LT_MAX_BASIC_SPRITES * 6];
		offset = 0;

		for (int i = 0; i < LT_MAX_TEXT_SPRITES * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		// create bindables
		s_TextRenderer.shader = Shader::Create(TextShaderSrc_VS, TextShaderSrc_FS);
		s_TextRenderer.vertexBuffer = VertexBuffer::Create(nullptr, LT_MAX_TEXT_SPRITES * sizeof(float) * 9 * 4, sizeof(float) * 9);

		s_TextRenderer.vertexLayout = VertexLayout::Create(s_TextRenderer.shader,
		                                                   s_TextRenderer.vertexBuffer,
		                                                   { {"POSITION" , VertexElementType::Float2},
		                                                     {"COLOR"    , VertexElementType::Float4},
		                                                     {"TEXCOORDS", VertexElementType::Float3}, });

		s_TextRenderer.indexBuffer = IndexBuffer::Create(indices, LT_MAX_TEXT_SPRITES * sizeof(unsigned int) * 6);

		// free memory
		delete[] indices;
		//================== TEXT RENDERER ==================//
	}

	void Renderer::BeginFrame()
	{
		// set view projection buffer
		glm::mat4* map = (glm::mat4*)s_ViewProjBuffer->Map();
		*(map + 0) = s_Camera->GetView();
		*(map + 1) = s_Camera->GetProjection();
		s_ViewProjBuffer->UnMap();

		if (s_MSAAEnabled)
			s_MSAA->BindFrameBuffer();
		else if (!s_Framebuffers.empty())
			s_Framebuffers[0]->BindAsTarget();
	}

	void Renderer::BeginLayer()
	{
		s_QuadRenderer.mapCurrent = (float*)s_QuadRenderer.vertexBuffer->Map();
		s_QuadRenderer.mapEnd = s_QuadRenderer.mapCurrent + LT_MAX_BASIC_SPRITES * 9 * 4;

		s_TextRenderer.mapCurrent = (float*)s_TextRenderer.vertexBuffer->Map();
		s_TextRenderer.mapEnd = s_TextRenderer.mapCurrent + LT_MAX_TEXT_SPRITES * 9 * 4;
	}
	
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const TextureCoordinates* uv, const glm::vec4& tint)
	{
		// #todo: make VertexBuffer size dynamic
		if (s_QuadRenderer.mapCurrent == s_QuadRenderer.mapEnd)
		{
			LT_CORE_ERROR("Renderer::DrawQuad: calls to this function exceeded its limit: {}", LT_MAX_BASIC_SPRITES);

			EndLayer();
			BeginLayer();
		}

		// locals
		const float xMin = position.x;
		const float yMin = position.y;
		const float xMax = position.x + size.x;
		const float yMax = position.y + size.y;

		const TextureCoordinates str = *uv;


		// TOP_LEFT
		s_QuadRenderer.mapCurrent[0 + 0] = xMin;
		s_QuadRenderer.mapCurrent[1 + 0] = yMin;

		s_QuadRenderer.mapCurrent[2 + 0] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 0] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 0] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 0] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 0] = str.xMin;
		s_QuadRenderer.mapCurrent[7 + 0] = str.yMin;
		s_QuadRenderer.mapCurrent[8 + 0] = str.sliceIndex;

		// TOP_RIGHT
		s_QuadRenderer.mapCurrent[0 + 9] = xMax;
		s_QuadRenderer.mapCurrent[1 + 9] = yMin;

		s_QuadRenderer.mapCurrent[2 + 9] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 9] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 9] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 9] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 9] = str.xMax;
		s_QuadRenderer.mapCurrent[7 + 9] = str.yMin;
		s_QuadRenderer.mapCurrent[8 + 9] = str.sliceIndex;

		// BOTTOM_RIGHT
		s_QuadRenderer.mapCurrent[0 + 18] = xMax;
		s_QuadRenderer.mapCurrent[1 + 18] = yMax;

		s_QuadRenderer.mapCurrent[2 + 18] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 18] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 18] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 18] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 18] = str.xMax;
		s_QuadRenderer.mapCurrent[7 + 18] = str.yMax;
		s_QuadRenderer.mapCurrent[8 + 18] = str.sliceIndex;

		// BOTTOM_LEFT
		s_QuadRenderer.mapCurrent[0 + 27] = xMin;
		s_QuadRenderer.mapCurrent[1 + 27] = yMax;

		s_QuadRenderer.mapCurrent[2 + 27] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 27] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 27] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 27] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 27] = str.xMin;
		s_QuadRenderer.mapCurrent[7 + 27] = str.yMax;
		s_QuadRenderer.mapCurrent[8 + 27] = str.sliceIndex;


		// move buffer map forward and increase quad count
		s_QuadRenderer.mapCurrent += 36;
		s_QuadRenderer.quadCount++;
	}

	void Renderer::DrawString(const std::string& text, const std::shared_ptr<Font>& font, const glm::vec2& position, float scale, const glm::vec4& tint)
	{
		unsigned int advance = 0;

		// loop through the string characters to render
		for (const auto& ch : text)
		{
			// #todo: make VertexBuffer size dynamic
			if (s_TextRenderer.mapCurrent == s_TextRenderer.mapEnd)
			{
				LT_CORE_ERROR("Renderer::DrawString: calls to this function exceeded its limit: {}", LT_MAX_TEXT_SPRITES);

				EndLayer();
				BeginLayer();
			}


			// locals
			const FontCharData character = *font->GetCharacterData(ch);

			const float xMin = position.x + (character.bearing.x * scale) + advance;
			const float yMin = position.y - (character.bearing.y) * scale;

			const float xMax = xMin + character.size.x * scale;
			const float yMax = yMin + character.size.y * scale;

			advance += (character.advance >> 6) * scale;

			const TextureCoordinates str = character.coordinates;


			// TOP_LEFT
			s_TextRenderer.mapCurrent[0 + 0] = xMin;
			s_TextRenderer.mapCurrent[1 + 0] = yMin;

			s_TextRenderer.mapCurrent[2 + 0] = tint.r;
			s_TextRenderer.mapCurrent[3 + 0] = tint.g;
			s_TextRenderer.mapCurrent[4 + 0] = tint.b;
			s_TextRenderer.mapCurrent[5 + 0] = tint.a;

			s_TextRenderer.mapCurrent[6 + 0] = str.xMin;
			s_TextRenderer.mapCurrent[7 + 0] = str.yMin;
			s_TextRenderer.mapCurrent[8 + 0] = str.sliceIndex;

			// TOP_RIGHT
			s_TextRenderer.mapCurrent[0 + 9] = xMax;
			s_TextRenderer.mapCurrent[1 + 9] = yMin;

			s_TextRenderer.mapCurrent[2 + 9] = tint.r;
			s_TextRenderer.mapCurrent[3 + 9] = tint.g;
			s_TextRenderer.mapCurrent[4 + 9] = tint.b;
			s_TextRenderer.mapCurrent[5 + 9] = tint.a;

			s_TextRenderer.mapCurrent[6 + 9] = str.xMax;
			s_TextRenderer.mapCurrent[7 + 9] = str.yMin;
			s_TextRenderer.mapCurrent[8 + 9] = str.sliceIndex;

			// BOTTOM_RIGHT
			s_TextRenderer.mapCurrent[0 + 18] = xMax;
			s_TextRenderer.mapCurrent[1 + 18] = yMax;

			s_TextRenderer.mapCurrent[2 + 18] = tint.r;
			s_TextRenderer.mapCurrent[3 + 18] = tint.g;
			s_TextRenderer.mapCurrent[4 + 18] = tint.b;
			s_TextRenderer.mapCurrent[5 + 18] = tint.a;

			s_TextRenderer.mapCurrent[6 + 18] = str.xMax;
			s_TextRenderer.mapCurrent[7 + 18] = str.yMax;
			s_TextRenderer.mapCurrent[8 + 18] = str.sliceIndex;

			// BOTTOM_LEFT
			s_TextRenderer.mapCurrent[0 + 27] = xMin;
			s_TextRenderer.mapCurrent[1 + 27] = yMax;

			s_TextRenderer.mapCurrent[2 + 27] = tint.r;
			s_TextRenderer.mapCurrent[3 + 27] = tint.g;
			s_TextRenderer.mapCurrent[4 + 27] = tint.b;
			s_TextRenderer.mapCurrent[5 + 27] = tint.a;

			s_TextRenderer.mapCurrent[6 + 27] = str.xMin;
			s_TextRenderer.mapCurrent[7 + 27] = str.yMax;
			s_TextRenderer.mapCurrent[8 + 27] = str.sliceIndex;


			// move buffer map forward and increase quad count
			s_TextRenderer.mapCurrent += 36;
			s_TextRenderer.quadCount++;
		}
	}

	void Renderer::EndLayer()
	{
		// unmap vertex buffers
		s_QuadRenderer.vertexBuffer->UnMap();
		s_TextRenderer.vertexBuffer->UnMap();

		// #todo: right now all texts will be rendered after basic quads, implement something to make them render in order they are called.
		//=============== BASIC QUAD RENDERER ===============//
		if (s_QuadRenderer.quadCount)
		{
			// bindables
			s_QuadRenderer.shader->Bind();
			s_QuadRenderer.vertexLayout->Bind();
			s_QuadRenderer.indexBuffer->Bind();
			s_QuadRenderer.vertexBuffer->Bind();

			// draw
			RenderCommand::DrawIndexed(s_QuadRenderer.quadCount * 6);
			s_QuadRenderer.quadCount = 0;
		}
		//=============== BASIC QUAD RENDERER ===============//

		//================== TEXT RENDERER ==================//
		if (s_TextRenderer.quadCount)
		{
			// bindables
			FontManager::BindTextureArray();

			s_TextRenderer.shader->Bind();
			s_TextRenderer.vertexLayout->Bind();
			s_TextRenderer.indexBuffer->Bind();
			s_TextRenderer.vertexBuffer->Bind();

			// draw
			RenderCommand::DrawIndexed(s_TextRenderer.quadCount * 6);
			s_TextRenderer.quadCount = 0;
		}
		//================== TEXT RENDERER ==================//
	}

	void Renderer::EndFrame()
	{
		// handle the framebuffers
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
		LT_CORE_ASSERT((sampleCount & (sampleCount - 1)) == 0, "Renderer::Init: MSAASampleCount is not power of 2");
		s_MSAA = MSAA::Create(sampleCount);
	}

} 