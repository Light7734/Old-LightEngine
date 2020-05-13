#pragma once

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

#include <unordered_map>

namespace Light {

	struct FontCharData
	{
		glm::vec2 size;
		glm::vec2 bearing;
		unsigned int advance;

		TextureCoordinates coordinates;
	};

	class Font
	{
	private:
		std::unordered_map<char, FontCharData> m_CharactersData;

		std::shared_ptr<TextureArray> m_TextureArrayRef;
		unsigned int m_SliceIndex;
	public:
		Font(std::shared_ptr<TextureArray> textureArray, const std::string& path, unsigned int size);
		~Font();

		const FontCharData& GetCharacterData(char character) { return m_CharactersData[character]; }
	};

}