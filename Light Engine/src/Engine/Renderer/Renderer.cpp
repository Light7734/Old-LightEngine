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

	std::vector<std::shared_ptr<Texture>> Renderer::s_Textures;

	std::map<std::shared_ptr<Texture>, unsigned int> Renderer::s_AttachmentCount;

	std::shared_ptr<Texture>              Renderer::s_WhiteTexture;
	unsigned int                          Renderer::s_CurrentTextureIndex = 0;

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
		                                                                          {"TEXCOORDS", VertexType::Float2},
		                                                                          {"TEXINDEX" , VertexType::UInt  }, }));

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


		// White texture
		unsigned char whiteTextureData[] = { 255, 255, 255, 255 };
		s_WhiteTexture = Texture::Create(whiteTextureData, 1, 1, 4);
		AttachTexture(s_WhiteTexture);
	}

	void Renderer::Start()
	{
		s_QuadRenderer.mapCurrent = (float*)s_QuadRenderer.vertexBuffer->Map();
		s_QuadRenderer.mapEnd = s_QuadRenderer.mapCurrent + LT_MAX_BASIC_SPRITES * 9 * 4;
	}
	
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
	                        const std::shared_ptr<Texture>& texture, const TextureCoordinates& textureCoordinates,
	                        const glm::vec4& tint)
	{
		// #todo: make VertexBuffer size dynamic
		if (s_QuadRenderer.mapCurrent == s_QuadRenderer.mapEnd)
		{
			LT_CORE_WARN("Too many Render::DrawQuad calls!");
			End();
			Start();
		}

		// Bind texture
		if (!texture->IsBonud())
		{
			if (s_CurrentTextureIndex > 15)
			{
				LT_CORE_WARN("Too many textures for a rendering stage!");
				End();
				Start();
			}

			texture->Bind(s_CurrentTextureIndex++);
		}

		// Locals
		unsigned int* const uintMap = (unsigned int*)s_QuadRenderer.mapCurrent;

		const float xMin = position.x;
		const float yMin = position.y;
		const float xMax = position.x + size.x;
		const float yMax = position.y + size.y;

		const unsigned int textureIndex = texture->GetIndex();


		// TOP_LEFT
		s_QuadRenderer.mapCurrent[0 + 0] = xMin;
		s_QuadRenderer.mapCurrent[1 + 0] = yMin;

		s_QuadRenderer.mapCurrent[2 + 0] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 0] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 0] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 0] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 0] = textureCoordinates.xMin;
		s_QuadRenderer.mapCurrent[7 + 0] = textureCoordinates.yMin;

		uintMap[8 + 0] = textureIndex;

		// TOP_RIGHT
		s_QuadRenderer.mapCurrent[0 + 9] = xMax;
		s_QuadRenderer.mapCurrent[1 + 9] = yMin;

		s_QuadRenderer.mapCurrent[2 + 9] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 9] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 9] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 9] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 9] = textureCoordinates.xMax;
		s_QuadRenderer.mapCurrent[7 + 9] = textureCoordinates.yMin;

		uintMap[8 + 9] = textureIndex;

		// BOTTOM_RIGHT
		s_QuadRenderer.mapCurrent[0 + 18] = xMax;
		s_QuadRenderer.mapCurrent[1 + 18] = yMax;

		s_QuadRenderer.mapCurrent[2 + 18] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 18] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 18] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 18] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 18] = textureCoordinates.xMax;
		s_QuadRenderer.mapCurrent[7 + 18] = textureCoordinates.yMax;

		uintMap[8 + 18] = textureIndex;

		// BOTTOM_LEFT
		s_QuadRenderer.mapCurrent[0 + 27] = xMin;
		s_QuadRenderer.mapCurrent[1 + 27] = yMax;

		s_QuadRenderer.mapCurrent[2 + 27] = tint.r;
		s_QuadRenderer.mapCurrent[3 + 27] = tint.g;
		s_QuadRenderer.mapCurrent[4 + 27] = tint.b;
		s_QuadRenderer.mapCurrent[5 + 27] = tint.a;

		s_QuadRenderer.mapCurrent[6 + 27] = textureCoordinates.xMin;
		s_QuadRenderer.mapCurrent[7 + 27] = textureCoordinates.yMax;

		uintMap[8 + 27] = textureIndex;


		// Increase buffer map and quad count
		s_QuadRenderer.mapCurrent += 36;
		s_QuadRenderer.quadCount++;
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
	                        const glm::vec4& color)
	{
		DrawQuad(position, size, s_WhiteTexture, { 0.0f, 0.0f, 1.0f, 1.0f }, color);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
	                        const std::shared_ptr<Texture>& texture, const TextureCoordinates& textureCoordinates)
	{
		DrawQuad(position, size, texture, textureCoordinates, {1.0f, 1.0f, 1.0f, 1.0f});
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

		// Reset textures binding status
		for (auto texture : s_Textures)
			texture->UnBind();
		s_CurrentTextureIndex = 0;
	}

	void Renderer::AttachTexture(std::shared_ptr<Texture> texture)
	{
		auto it = std::find(s_Textures.begin(), s_Textures.end(), texture);

		if (it != s_Textures.end())
			s_AttachmentCount[texture]++;
		else
		{
			s_AttachmentCount[texture] = 1u;
			s_Textures.push_back(texture);
		}
	}

	void Renderer::DetatchTexture(std::shared_ptr<Texture> texture)
	{
		// #todo: make sure DetatchTexture is called after AttachTexture 
		auto it = std::find(s_Textures.begin(), s_Textures.end(), texture);

		if (it != s_Textures.end())
		{
			if (--s_AttachmentCount[texture] == 0)
			{
				s_Textures.erase(it);
				s_AttachmentCount.erase(texture);
			}
		}
		else
			LT_CORE_ERROR("Failed to find specified texture (Renderer::DetatchTexture)");
	}

}