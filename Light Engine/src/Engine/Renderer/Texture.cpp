#include "ltpch.h"
#include "Texture.h"

#include "GraphicsContext.h"

#include "Utility/FileManager.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxTexture.h"
#endif
#include "Platform/Opengl/glTexture.h"

namespace Light {
	
	Texture::Texture(const std::string& atlasPath, const SubTexture& texture, const SubTexture& slice)
		: m_Texture(texture)
	{
		LT_PROFILE_FUNC();

		std::string atlas = FileManager::LoadTextFile(atlasPath);

		std::stringstream stream(atlas);
		std::string line;

		float xRatio = (texture.xMax - texture.xMin) / slice.xMax;
		float yRatio = (texture.yMax - texture.yMin) / slice.yMax;

		float xOffset = texture.xMin * xRatio;
		float yOffset = texture.yMin * yRatio;

		// note: I'm using (CodeAndWeb)TexturePacker with a custom exporter
		while (std::getline(stream, line))
		{
			std::istringstream lineStream(line);
			std::string temp;

			std::string name;
			float xMin, yMin, xMax, yMax;

			std::getline(lineStream, name, ' ');
			std::getline(lineStream, temp, ' '); xMin = std::stof(temp);
			std::getline(lineStream, temp, ' '); yMin = std::stof(temp);
			std::getline(lineStream, temp, ' '); xMax = std::stof(temp); xMax += xMin;
			std::getline(lineStream, temp, ' '); yMax = std::stof(temp); yMax += yMin;

			m_SubTextures[name] = { xMin * xRatio + xOffset,
			                        yMin * yRatio + yOffset,
			                        xMax * xRatio + xOffset,
			                        yMax * yRatio + yOffset,
			                        texture.sliceIndex };
		}
	}

	Texture::Texture(const SubTexture& texture)
		: m_Texture(texture)
	{
	}

	TextureArray::TextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels)
		: m_Width(width), m_Height(height), m_Depth(depth), m_Channels(channels)
	{
		m_Pixels.resize(depth);
	}

	std::shared_ptr<Light::TextureArray> TextureArray::Create(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels /*= 4*/)
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glTextureArray>(width, height, depth, channels);
		case GraphicsAPI::Directx: LT_DX(
			return std::make_shared<dxTextureArray>(width, height, depth, channels);)
		default:
			LT_CORE_ASSERT(false, "TextureArray::Create: invalid GraphicsAPI");
		}
	}

	void TextureArray::LoadTexture(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		m_UnresolvedTextures.push_back({ name, atlasPath, FileManager::LoadTextureFile(texturePath) });
	}

	void TextureArray::LoadTexture(const std::string& name, const std::string& texturePath)
	{
		m_UnresolvedTextures.push_back( { name, "", FileManager::LoadTextureFile(texturePath)} );
	}

	void TextureArray::LoadTexture(const std::string& name, unsigned width, unsigned int height)
	{
		m_UnresolvedTextures.push_back({ name, "", TextureImageData(nullptr, width, height, m_Channels) });
	}

	void TextureArray::ResolveTextures()
	{
		bool found = false;

		std::sort(m_UnresolvedTextures.begin(), m_UnresolvedTextures.end(), std::greater());

		for (auto& data : m_UnresolvedTextures)
		{
			auto& t = data.texture;

			for (int z = 0; z < m_Depth; z++)
			{
				for (int y = 0; y < m_Height - t.height && !found; y++)
				{
					for (int x = 0; x <= m_Width - t.width && !found; x++)
					{
						bool valid = !(m_Pixels[z][       y        ][       x       ] ||
							           m_Pixels[z][       y        ][x + t.width - 1] ||
							           m_Pixels[z][y + t.height - 1][       x       ] ||
							           m_Pixels[z][y + t.height - 1][x + t.width - 1]);

						if (valid)
							for (int yi = 0; yi < t.height - 1 && valid; yi++)
								for (int xi = 0; xi < t.width - 1; xi++)
									if (m_Pixels[z][y + yi][x + xi])
										{ valid = false; break; }

						if (valid)
						{
							found = true;

							for (int yi = 0; yi < t.height - 1 && valid; yi++)
								for (int xi = 0; xi < t.width - 1; xi++)
									m_Pixels[z][y + yi][x + xi] = true;

							if(t.pixels)
								UpdateSubTexture(x, y, 0u, t.width, t.height, t.pixels);

							if(!data.atlasPath.empty())
								m_Textures[data.name] = std::make_shared<Texture>(data.atlasPath, SubTexture(x, y, x + t.width, y + t.height, z),
								                                                                  SubTexture(0, 0, m_Width, m_Height, z));
							else
								m_Textures[data.name] = std::make_shared<Texture>(SubTexture(x, y, x + t.width, y + t.height, z));
						}
					}
				}
			}

			LT_CORE_ASSERT(found, "TextureArray::ResolveTextures: could not find a valid space for texture");
			found = false;
			free(t.pixels);
		}
		m_UnresolvedTextures.clear();
		GenerateMips();
	}

	void TextureArray::DeleteTexture(const std::string& name)
	{
		if (m_Textures.find(name) != m_Textures.end())
		{
			m_Textures.erase(name);
		}
		else
			LT_CORE_ERROR("TextureArray::DeleteSlice: failed to find slice: {}", name);
	}
	
}	