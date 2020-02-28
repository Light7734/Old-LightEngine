#include "ltpch.h"
#include "Texture.h"

#include "GraphicsContext.h"

#include "Utility/FileManager.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxTexture.h"
#endif
#include "Platform/Opengl/glTexture.h"

namespace Light {

	std::vector<unsigned int> TextureAtlas::s_AvailableSlots = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	unsigned int TextureAtlas::s_Height = 0;
	unsigned int TextureAtlas::s_Width = 0;

	TextureAtlas::TextureAtlas()
	{
		LT_CORE_ASSERT(!s_AvailableSlots.empty(), "TextureAtlas::TextureAtlas: Texture atlas count exceeds the limit: {}", 16);

		m_Index = s_AvailableSlots.back();
		s_AvailableSlots.pop_back();
	}

	TextureAtlas::~TextureAtlas()
	{
		s_AvailableSlots.push_back(m_Index);
	}

	std::shared_ptr<Light::TextureAtlas> TextureAtlas::Create(const std::string& atlasPath)
	{
		std::string atlasData = FileManager::LoadTextFile(atlasPath);
		TextureData data;
		data.pixels = FileManager::LoadTextureFile(atlasData.substr(0, atlasData.find('\n')),
		                                           &data.width, &data.height, &data.channels);

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
		{
			std::shared_ptr<glTextureAtlas> glAtlas = std::make_shared<glTextureAtlas>(data);

			glAtlas->ParseSegments(atlasData);
			return glAtlas;
		}
		case GraphicsAPI::Directx: LT_DX(
		{
			std::shared_ptr<dxTextureAtlas> dxAtlas = std::make_shared<dxTextureAtlas>(data);

			dxAtlas->ParseSegments(atlasData);
			return dxAtlas;
		} )
		default:
			LT_CORE_ASSERT(false, "TextureAtlas::Create: Invalid GraphicsAPI");
		}
	}

	void TextureAtlas::ParseSegments(const std::string& data)
	{
		std::stringstream stream(data);
		std::string line;

		std::getline(stream, line); // skip the first line
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

			m_Segments[name] = { xMin, yMin, xMax, yMax, static_cast<float>(m_Index) };
		}
	}
	
	void TextureAtlas::DestroyTextureArray()
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
		{
			glTextureAtlas::DestroyTextureArray();
			break;
		}
		case GraphicsAPI::Directx: LT_DX(
		{
			dxTextureAtlas::DestroyTextureArray();
			break;
		})
		default:
			LT_CORE_ASSERT(false, "TextureAtlas::DestroyTextureArray: Invalid GraphicsAPI");
		}
	}

}	