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

		TextureCoordinates glyph;
	};

	class Font
	{
	private:
		std::unordered_map<char, FontCharData> m_CharactersData;
	public:
		Font(const std::string& name, const std::string& path, std::shared_ptr<TextureArray> textureArray, unsigned int size);

		inline const FontCharData& GetCharacterData(char character) const { return m_CharactersData.at(character); }
	};

}