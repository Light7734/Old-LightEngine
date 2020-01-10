#include <ltpch.h>
#include "Renderer.h"

#include "RenderCommand.h"
#include "VertexLayout.h"

#include "Shaders/QuadShader.h"

#define LT_INIT_SPRITES 10000

namespace Light {

	std::shared_ptr<Light::Shader> Renderer::s_Shader;
	std::shared_ptr<Light::VertexBuffer> Renderer::s_Buffer;
	std::shared_ptr<Light::IndexBuffer> Renderer::s_Indices;
	std::shared_ptr<Light::BufferLayout> Renderer::s_Layout;

	unsigned int Renderer::s_QuadCount = 0;

	float* Renderer::s_BufferMapStart;
	float* Renderer::s_BufferMapCurrent;
	float* Renderer::s_BufferMapEnd;

	void Renderer::Init()
	{
		// Create Shader
		s_Shader = Shader::Create(QuadShaderSrc_VS, QuadShaderSrc_FS);

		// Create VertexBuffer
		s_Buffer = VertexBuffer::Create(nullptr, LT_INIT_SPRITES * sizeof(float) * 6 * 4, sizeof(float) * 6);

		// Create indices for IndexBuffer
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

		// Create IndexBuffer and release memory
		s_Indices = IndexBuffer::Create(indices, LT_INIT_SPRITES * sizeof(unsigned int) * 6);
		delete indices;

		// Set VertexBuffer's layout
		std::shared_ptr<VertexLayout> vertexLayout = VertexLayout::Create({ {"Position", VertexType::Float2},
		                                                                    { "Color"  , VertexType::Float4 } });
		s_Layout = BufferLayout::Create(s_Shader, s_Buffer, vertexLayout);

		s_BufferMapStart = (float*)s_Buffer->Map();
		s_BufferMapEnd = s_BufferMapStart + LT_INIT_SPRITES * 6 * 4;
	}

	void Renderer::Start()
	{
		s_BufferMapCurrent = s_BufferMapStart;
	}
	
	void Renderer::DrawQuad(float x, float y, float width, float height)
	{
		if (s_BufferMapCurrent == s_BufferMapEnd)
		{
			LT_CORE_ASSERT(false, EC_ENGINE_ASSERTION, "Too many DrawQuad calls!");
		}

		s_BufferMapCurrent[0] = x;
		s_BufferMapCurrent[1] = y;

		s_BufferMapCurrent[2] = 1.0f;
		s_BufferMapCurrent[3] = 0.0f;
		s_BufferMapCurrent[4] = 1.0f;
		s_BufferMapCurrent[5] = 1.0f;


		s_BufferMapCurrent[0 + 6] = x;
		s_BufferMapCurrent[1 + 6] = y + height;

		s_BufferMapCurrent[2 + 6] = 1.0f;
		s_BufferMapCurrent[3 + 6] = 1.0f;
		s_BufferMapCurrent[4 + 6] = 0.0f;
		s_BufferMapCurrent[5 + 6] = 1.0f;


		s_BufferMapCurrent[0 + 12] = x + width;
		s_BufferMapCurrent[1 + 12] = y + height;

		s_BufferMapCurrent[2 + 12] = 1.0f;
		s_BufferMapCurrent[3 + 12] = 0.0f;
		s_BufferMapCurrent[4 + 12] = 1.0f;
		s_BufferMapCurrent[5 + 12] = 1.0f;


		s_BufferMapCurrent[0 + 18] = x + width;
		s_BufferMapCurrent[1 + 18] = y ;

		s_BufferMapCurrent[2 + 18] = 1.0f;
		s_BufferMapCurrent[3 + 18] = 1.0f;
		s_BufferMapCurrent[4 + 18] = 1.0f;
		s_BufferMapCurrent[5 + 18] = 1.0f;

		s_BufferMapCurrent += sizeof(float) * 6 * 4;
		
		s_QuadCount++;
	}

	void Renderer::End()
	{
		s_Shader->Bind();
		s_Layout->Bind();

		s_Buffer->Bind();
		s_Buffer->UnMap();
		s_Indices->Bind();

		RenderCommand::DrawIndexed(s_QuadCount * 6);
		s_QuadCount = 0;

		s_BufferMapCurrent = s_BufferMapStart;
	}

}