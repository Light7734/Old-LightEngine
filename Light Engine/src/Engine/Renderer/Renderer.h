#pragma once

#include "Core/Core.h"

#include "Texture.h"

#include <glm/glm.hpp>

#include <map>

namespace Light {

	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class BufferLayout;
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

		static std::vector<std::shared_ptr<Texture>> s_Textures;
		static std::map<std::shared_ptr<Texture>, unsigned int> s_AttachmentCount; // #todo: maybe map uid of texture?

		static std::shared_ptr<Texture> s_WhiteTexture;

		static unsigned int s_CurrentTextureIndex;
	public:
		static void Init();

		static void Start();

		//=============== BASIC QUAD RENDERER ===============//
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
		                     const std::shared_ptr<Texture>& texture, const TextureCoordinates& textureCoordinates,
		                     const glm::vec4& tint);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
		                     const std::shared_ptr<Texture>& texture, const TextureCoordinates& textureCoordinates);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
		                     const glm::vec4& color);
		//=============== BASIC QUAD RENDERER ===============//

		static void End();

		static void AttachTexture(std::shared_ptr<Texture> texture);
		static void DetatchTexture(std::shared_ptr<Texture> texture);
	};

}