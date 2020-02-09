#include <ltpch.h>
#include "Renderer.h"

#include "BufferLayout.h"
#include "Buffers.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexLayout.h"

#include "Shaders/QuadShader.h"

#define LT_MAX_BASIC_SPRITES 10000

namespace Light {

	Renderer::BasicQuadRenderer Renderer::s_QuadRenderer;

	void Renderer::Init()
	{
		//=============== BASIC QUAD RENDERER ===============//
		// Create stuff 
		s_QuadRenderer.shader = Shader::Create(QuadShaderSrc_VS, QuadShaderSrc_FS);
		s_QuadRenderer.vertexBuffer = VertexBuffer::Create(nullptr, LT_MAX_BASIC_SPRITES * sizeof(float) * 9 * 4, sizeof(float) * 9);

		s_QuadRenderer.bufferLayout = BufferLayout::Create(s_QuadRenderer.shader,
		                                                   s_QuadRenderer.vertexBuffer,
		                                                   VertexLayout::Create({ {"POSITION" , VertexType::Float2},
		                                                                          {"COLOR"    , VertexType::Float4},
		                                                                          {"TEXCOORDS", VertexType::Float3}, }));

		// Indices for index buffer
		unsigned int* indices = new unsigned int [LT_MAX_BASIC_SPRITES * 6];
		unsigned int offset = 0;

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

		// Create IndexBuffer
		s_QuadRenderer.indexBuffer = IndexBuffer::Create(indices, LT_MAX_BASIC_SPRITES * sizeof(unsigned int) * 6);
		delete[] indices;
		//=============== BASIC QUAD RENDERER ===============//
	}

	void Renderer::Start()
	{
		s_QuadRenderer.mapCurrent = (float*)s_QuadRenderer.vertexBuffer->Map();
		s_QuadRenderer.mapEnd = s_QuadRenderer.mapCurrent + LT_MAX_BASIC_SPRITES * 9 * 4;
	}
	
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
	                        const TextureCoordinates* textureCoordinates, const glm::vec4& tint)
	{
		// #todo: make VertexBuffer size dynamic
		if (s_QuadRenderer.mapCurrent == s_QuadRenderer.mapEnd)
		{
			LT_CORE_WARN("Too many Render::DrawQuad calls!");
			End();
			Start();
		}

		// Locals
		const float xMin = position.x;
		const float yMin = position.y;
		const float xMax = position.x + size.x;
		const float yMax = position.y + size.y;

		const TextureCoordinates str = *textureCoordinates;

		// TOP_LEFT
		s_QuadRenderer.mapCurrent[0 + 0] = xMin;
		s_QuadRenderer.mapCurrent[1 + 0] = yMin;

		s_QuadRenderer.mapCurrent[2 + 0] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 0] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 0] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 0] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 0] = str.xMin;
		s_QuadRenderer.mapCurrent[7 + 0] = str.yMin;
		s_QuadRenderer.mapCurrent[8 + 0] = str.atlasIndex;

		// TOP_RIGHT
		s_QuadRenderer.mapCurrent[0 + 9] = xMax;
		s_QuadRenderer.mapCurrent[1 + 9] = yMin;

		s_QuadRenderer.mapCurrent[2 + 9] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 9] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 9] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 9] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 9] = str.xMax;
		s_QuadRenderer.mapCurrent[7 + 9] = str.yMin;
		s_QuadRenderer.mapCurrent[8 + 9] = str.atlasIndex;

		// BOTTOM_RIGHT
		s_QuadRenderer.mapCurrent[0 + 18] = xMax;
		s_QuadRenderer.mapCurrent[1 + 18] = yMax;

		s_QuadRenderer.mapCurrent[2 + 18] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 18] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 18] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 18] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 18] = str.xMax;
		s_QuadRenderer.mapCurrent[7 + 18] = str.yMax;
		s_QuadRenderer.mapCurrent[8 + 18] = str.atlasIndex;

		// BOTTOM_LEFT
		s_QuadRenderer.mapCurrent[0 + 27] = xMin;
		s_QuadRenderer.mapCurrent[1 + 27] = yMax;

		s_QuadRenderer.mapCurrent[2 + 27] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 27] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 27] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 27] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 27] = str.xMin;
		s_QuadRenderer.mapCurrent[7 + 27] = str.yMax;
		s_QuadRenderer.mapCurrent[8 + 27] = str.atlasIndex;


		// Increase buffer map and quad count
		s_QuadRenderer.mapCurrent += 36;
		s_QuadRenderer.quadCount++;
	}

	void Renderer::End()
	{
		// Unmap vertex buffers
		s_QuadRenderer.vertexBuffer->UnMap();

		//=============== BASIC QUAD RENDERER ===============//
		if (s_QuadRenderer.quadCount)
		{
			// Bind stuff
			s_QuadRenderer.shader->Bind();
			s_QuadRenderer.bufferLayout->Bind();
			s_QuadRenderer.indexBuffer->Bind();
			s_QuadRenderer.vertexBuffer->Bind();

			// Draw
			RenderCommand::DrawIndexed(s_QuadRenderer.quadCount* 6);
			s_QuadRenderer.quadCount = 0;
		}
		//=============== BASIC QUAD RENDERER ===============//
	}

}