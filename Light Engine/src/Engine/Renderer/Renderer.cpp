#include <ltpch.h>
#include "Renderer.h"

#include "BufferLayout.h"
#include "Buffers.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexLayout.h"

#include "Shaders/QuadShader.h"

#define LT_INIT_SPRITES 10000

namespace Light {

	std::shared_ptr<Shader>       Renderer::s_Shader      ;
	std::shared_ptr<VertexBuffer> Renderer::s_VertexBuffer;
	std::shared_ptr<IndexBuffer>  Renderer::s_Indices     ;
	std::shared_ptr<BufferLayout> Renderer::s_Layout      ;

	unsigned int Renderer::s_QuadCount = 0;

	float* Renderer::s_BufferMapCurrent;
	float* Renderer::s_BufferMapEnd    ;

	void Renderer::Init()
	{
		// Create stuff 
		s_Shader = Shader::Create(QuadShaderSrc_VS, QuadShaderSrc_FS);
		s_VertexBuffer = VertexBuffer::Create(nullptr, LT_INIT_SPRITES * sizeof(float) * 6 * 4, sizeof(float) * 6);

		s_Layout = BufferLayout::Create(s_Shader, s_VertexBuffer, VertexLayout::Create({ {"POSITION", VertexType::Float2},
		                                                                                 {"COLOR"   , VertexType::Float4 } }));
		// Indices for index buffer
		unsigned int* indices = new unsigned int [LT_INIT_SPRITES * 6];
		unsigned int offset = 0;

		for (int i = 0; i < LT_INIT_SPRITES * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 6;
		}

		// Create IndexBuffer
		s_Indices = IndexBuffer::Create(indices, LT_INIT_SPRITES * sizeof(unsigned int) * 6);
		delete[] indices;
	}

	void Renderer::Start()
	{
		s_BufferMapCurrent = (float*)s_VertexBuffer->Map();
		s_BufferMapEnd = s_BufferMapCurrent + LT_INIT_SPRITES * 6 * 4;
	}
	
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		// #todo: make VertexBuffer size dynamic
		LT_CORE_ASSERT(s_BufferMapCurrent != s_BufferMapEnd, "Too many Renderer::DrawQuad calls");

		float xMin = position.x;
		float yMin = position.y;
		float xMax = position.x + size.x;
		float yMax = position.y + size.y;


		// TOP_LEFT
		s_BufferMapCurrent[0] = xMin;
		s_BufferMapCurrent[1] = yMin;

		s_BufferMapCurrent[2] = color.r;
		s_BufferMapCurrent[3] = color.g;
		s_BufferMapCurrent[4] = color.b;
		s_BufferMapCurrent[5] = color.a;


		// TOP_RIGHT
		s_BufferMapCurrent[0 + 6] = xMax;
		s_BufferMapCurrent[1 + 6] = yMin;

		s_BufferMapCurrent[2 + 6] = color.r;
		s_BufferMapCurrent[3 + 6] = color.g;
		s_BufferMapCurrent[4 + 6] = color.b;
		s_BufferMapCurrent[5 + 6] = color.a;


		// BOTTOM_RIGHT
		s_BufferMapCurrent[0 + 12] = xMax;
		s_BufferMapCurrent[1 + 12] = yMax;

		s_BufferMapCurrent[2 + 12] = color.r;
		s_BufferMapCurrent[3 + 12] = color.g;
		s_BufferMapCurrent[4 + 12] = color.b;
		s_BufferMapCurrent[5 + 12] = color.a;


		// BOTTOM_LEFT
		s_BufferMapCurrent[0 + 18] = xMin;
		s_BufferMapCurrent[1 + 18] = yMax;

		s_BufferMapCurrent[2 + 18] = color.r;
		s_BufferMapCurrent[3 + 18] = color.g;
		s_BufferMapCurrent[4 + 18] = color.b;
		s_BufferMapCurrent[5 + 18] = color.a;

		// Increase buffer map and quad count
		s_BufferMapCurrent += sizeof(float) * 6 * 4;
		s_QuadCount++;
	}

	void Renderer::End()
	{
		// Bind stuff
		s_Shader->Bind();
		s_Layout->Bind();
		s_Indices->Bind();
		s_VertexBuffer->Bind();

		// Draw
		s_VertexBuffer->UnMap();
		RenderCommand::DrawIndexed(s_QuadCount * 6);
		s_QuadCount = 0;
	}

}