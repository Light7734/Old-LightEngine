#include "ltpch.h"
#include "Font.h"

namespace Light {

	Font::Font(const std::string& name, const std::string& path, std::shared_ptr<TextureArray> textureArray, unsigned int size)
	{
		LT_PROFILE_FUNC();

		std::vector<TextureCoordinates> glyphsSpace;

		float width = 0.0f;
		float height = 0.0f;
		
		std::vector<unsigned char*> buffers(128);
		buffers.resize(128);

		FontFileData font = FileManager::LoadFont(path, size);

		for (uint8_t i = 0; i < 128; i++)
		{
			bool found = false;

			font.LoadChar(i);
			m_CharactersData[i] = { font.GetSize(), font.GetBearing(), font.GetAdvance() };

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
			font.LoadChar(i);

			// write to texture slice
			textureArray->UpdateSubTexture(glyphsSpace[i].xMin + xOffset, glyphsSpace[i].yMin + yOffset, coords.sliceIndex,
			                               glyphsSpace[i].GetWidth(), glyphsSpace[i].GetHeight(),
			                               font.GetBuffer());

			// figure out and set character's texture coordinates
			m_CharactersData.at(i).glyphUV = { tcuX * (glyphsSpace[i].xMin + xOffset), tcuY * (glyphsSpace[i].yMin + yOffset), // xMin, yMin
								          tcuX * (glyphsSpace[i].xMax + xOffset), tcuY * (glyphsSpace[i].yMax + yOffset), // xMax, yMax
			                              coords.sliceIndex }; // sliceIndex
		}
		textureArray->GenerateMips();
	}

}