#include "ltpch.h"
#include "Font.h"

#include "Utility/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define LT_FONT_CHAR_PADDING 4u

namespace Light {

	Font::Font(std::shared_ptr<TextureArray> textureArray, SubTexture bounds, const std::string& path, unsigned int size)
	{
		LT_PROFILE_FUNC();

		// load font
		FT_Library library;
		FT_Face face;

		LT_CORE_ASSERT(!FT_Init_FreeType(&library), "Font::Font: FT_Init_FreeType failed");
		LT_CORE_ASSERT(!FT_New_Face(library, path.c_str(), 0, &face), "Font::Font: FT_New_Face failed");
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(face, 0, size), "Font::Font: FT_Set_Pixel_Sizes failed");

		// load characters
		float xOffset = bounds.xMin;
		float yOffset = bounds.yMin;

		float tcuX = 1.0f / textureArray->GetWidth();  // texture coordinates unit - x axis
		float tcuY = 1.0f / textureArray->GetHeight(); // texture coordinates unit - y axis

		float tcuXOffset = tcuX * bounds.xMin;
		float tcuYOffset = tcuX * bounds.yMin;

		unsigned int x = 0u, y = 0u;
		unsigned int prevY = 0u, nextY = 0u;

		for (int i = 0; i < 128; i++) // #todo: currently the generated glyph atlas will be a bit large due to novice texture packing algorithm
		{
			// load character from FT_Face
			LT_CORE_ASSERT(!FT_Load_Char(face, i, FT_LOAD_RENDER), "Font::Font: FT_Load_Char failed: {}", i);

			FontCharData character = { glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
									   glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
									   (unsigned int)face->glyph->advance.x >> 6 };

			// not enough space? go next line
			if (x + xOffset > bounds.xMax - character.size.x)
			{
				x = 0;
				prevY = y;
				y = nextY + LT_FONT_CHAR_PADDING;

				LT_CORE_ASSERT(y < bounds.yMax, "BLYAT");
			}

			// write to texture slice
			textureArray->UpdateSubTexture(x + xOffset, y + yOffset, bounds.sliceIndex, character.size.x, character.size.y, face->glyph->bitmap.buffer);

			// figure out and set character's texture coordinates
			character.glyph = { tcuX * (x + xOffset)                   , tcuY * (y + yOffset),                      // xMin, yMin
								tcuX * (x + character.size.x + xOffset), tcuY * (y + character.size.y + yOffset), // xMax, yMax
			                    (float)bounds.sliceIndex }; // sliceIndex

			// find the character with largest 'height', we will add that much to 'y' when we go next line
			if (nextY - prevY < character.size.y)
				nextY = y + character.size.y;

			// move 'x' forward
			x += character.size.x + LT_FONT_CHAR_PADDING;

			// set member
			m_CharactersData[i] = character;
		}
		textureArray->GenerateMips();

		// free memory
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

}