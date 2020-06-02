#pragma once

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

#include <unordered_map>

#include "Utility/FIleManager.h"

namespace Light {

	struct FontCharData
	{
		glm::vec2 size;
		glm::vec2 bearing;
		unsigned int advance;

		TextureCoordinates glyphUV;
		
		FontCharData() = default;
		FontCharData(FontCharGlyphData data) : size(data.size), bearing(data.bearing), advance(data.advance) { LT_CORE_DEBUG("AC"); }
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