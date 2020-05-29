#include "ltpch.h"
#include "Font.h"

#include "Utility/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define LT_FONT_CHAR_PADDING 4u

namespace Light {

	Font::Font(const std::string& name, const std::string& path, std::shared_ptr<TextureArray> textureArray, unsigned int size)
	{
		LT_PROFILE_FUNC();

		// load font
		FT_Library library;
		FT_Face face;

		LT_CORE_ASSERT(!FT_Init_FreeType(&library), "Font::Font: FT_Init_FreeType failed");
		LT_CORE_ASSERT(!FT_New_Face(library, path.c_str(), 0, &face), "Font::Font: FT_New_Face failed");
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(face, 0, size), "Font::Font: FT_Set_Pixel_Sizes failed");

		std::vector<TextureCoordinates> glyphsSpace;

		float width = 0.0f;
		float height = 0.0f;

		for (uint8_t i = 0; i < 128; i++)
		{
			LT_CORE_ASSERT(!FT_Load_Char(face, i, FT_LOAD_RENDER), "Font::Font: FT_Load_Char failed: {}", (char)i);

			bool found = false;
			m_CharactersData[i] = { glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			                        glm::vec2(face->glyph->bitmap_left , face->glyph->bitmap_top ),
			                        (unsigned int)face->glyph->advance.x >> 6 };

			for (uint16_t y = 0; y < 2048u && !found; y++)
			{
				found = false;
				for (uint16_t x = 0; x < 2048u && !found; x++)
				{
					const TextureCoordinates uv(x, y, x + m_CharactersData[i].size.x, y + m_CharactersData[i].size.y, 0);

					bool valid = true;
					for (const auto& t : glyphsSpace)
						if (t.Intersects(uv))
							{ valid = false; break; }

					if (valid)
					{
						found = true;
						glyphsSpace.push_back(uv);

						if (uv.xMax > width)
							width = uv.xMax;
						if (uv.yMax > height)
							height = uv.yMax;
					}
				}
			}
		}

		textureArray->AllocateTexture(name, width, height);
		textureArray->ResolveTextures();
		TextureCoordinates coords = *textureArray->GetTexture(name)->GetTextureUV();

		// load characters
		float xOffset = coords.xMin * textureArray->GetWidth();
		float yOffset = coords.yMin * textureArray->GetHeight();

		float tcuX = 1.0f / textureArray->GetWidth();  // texture coordinates unit - x axis
		float tcuY = 1.0f / textureArray->GetHeight(); // texture coordinates unit - y axis

		for (uint8_t i = 0; i < 128; i++)
		{
			// load character from FT_Face
			FT_Load_Char(face, i, FT_LOAD_RENDER);

			// write to texture slice
			glyphsSpace[i].sliceIndex = coords.sliceIndex;
			textureArray->UpdateSubTexture(glyphsSpace[i].xMin + xOffset, glyphsSpace[i].yMin + yOffset, glyphsSpace[i].sliceIndex,
			                               glyphsSpace[i].GetWidth(), glyphsSpace[i].GetHeight(),
			                               face->glyph->bitmap.buffer);

			// figure out and set character's texture coordinates
			m_CharactersData[i].glyph = { tcuX * (glyphsSpace[i].xMin + xOffset), tcuY * (glyphsSpace[i].yMin + yOffset), // xMin, yMin
								          tcuX * (glyphsSpace[i].xMax + xOffset), tcuY * (glyphsSpace[i].yMax + yOffset), // xMax, yMax
			                              (float)glyphsSpace[i].sliceIndex }; // sliceIndex
		}
		textureArray->GenerateMips();

		// free memory
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

}