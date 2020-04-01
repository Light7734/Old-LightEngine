#include "ltpch.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define LT_FONT_CHAR_PADDING 4u

namespace Light {

	Font::Font(std::shared_ptr<TextureArray> textureArray, const std::string& path, unsigned int size)
		: m_SliceIndex(0), m_TextureArrayRef(textureArray)
	{
		// load font
		FT_Library library;
		FT_Face face;

		LT_CORE_ASSERT(!FT_Init_FreeType(&library), "glFont::glFont: FT_Init_FreeType failed");
		LT_CORE_ASSERT(!FT_New_Face(library, path.c_str(), 0, &face), "glFont::glFont: FT_New_Face failed");
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(face, 0, size), "glFont::glFont: FT_Set_Pixel_Sizes failed");


		// create texture slice
		m_SliceIndex = textureArray->CreateSlice(1024, 1024, NULL);


		// load characters
		float tcu = 1.0f / 1024.0f; // texture coordinates unit

		unsigned int x = 0u, y = 0u;
		unsigned int prevY = 0u, nextY = 0u;

		textureArray->Bind();
		for (int i = 0; i < 128; i++)
		{
			// load character from FT_Face
			LT_CORE_ASSERT(!FT_Load_Char(face, i, FT_LOAD_RENDER), "glFont::glFont: FT_Load_Char failed: {}", i);

			FontCharData character = { glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
									   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
									   (unsigned int)face->glyph->advance.x };

			// not enough space? go next line
			if (x > 1024.0f - character.size.x)
			{
				x = 0;
				prevY = y;
				y = nextY + LT_FONT_CHAR_PADDING;
			}

			// write to texture slice
			textureArray->UpdateSubTexture(x, y, m_SliceIndex, character.size.x, character.size.y, face->glyph->bitmap.buffer);

			// figure out and set character's uv
			character.coordinates = { tcu * x, tcu * y, // xMin, yMin
									  tcu * (x + character.size.x), tcu * (y + character.size.y), // xMax, yMax
									  (float)m_SliceIndex }; // sliceIndex

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

	Font::~Font()
	{
		m_TextureArrayRef->DeleteSlice(m_SliceIndex);
	}

}