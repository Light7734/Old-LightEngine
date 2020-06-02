#include "ltpch.h"
#include "Font.h"

#include "Utility/ResourceManager.h"

#define LT_FONT_CHAR_PADDING 4u

namespace Light {

	Font::Font(const std::string& name, const std::string& path, std::shared_ptr<TextureArray> textureArray, unsigned int size)
	{
		LT_PROFILE_FUNC();

		FileManager::LoadFont(path, size);

		std::vector<TextureCoordinates> glyphsSpace;

		float width = 0.0f;
		float height = 0.0f;
		
		std::vector<unsigned char*> buffers(128);
		buffers.resize(128);

		for (uint8_t i = 0; i < 128; i++)
		{
			bool found = false;
			m_CharactersData[i] = FontCharData(FileManager::LoadFontCharGlyph(i, &buffers[i]));
						
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
			// write to texture slice
			FileManager::LoadFontCharGlyph(i, &buffers[i]);

			textureArray->UpdateSubTexture(glyphsSpace[i].xMin + xOffset, glyphsSpace[i].yMin + yOffset, coords.sliceIndex,
			                               glyphsSpace[i].GetWidth(), glyphsSpace[i].GetHeight(),
			                               buffers[i]);

			// figure out and set character's texture coordinates
			m_CharactersData.at(i).glyphUV = { tcuX * (glyphsSpace[i].xMin + xOffset), tcuY * (glyphsSpace[i].yMin + yOffset), // xMin, yMin
								          tcuX * (glyphsSpace[i].xMax + xOffset), tcuY * (glyphsSpace[i].yMax + yOffset), // xMax, yMax
			                              coords.sliceIndex }; // sliceIndex
		}
		textureArray->GenerateMips();

		// free memory
		FileManager::FreeLoadedFont();
	}

}