#include "ltpch.h"
#include "Texture.h"

#include "GraphicsContext.h"

#include "Utility/FileManager.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxTexture.h"
#endif
#include "Platform/Opengl/glTexture.h"

namespace Light {
	
	TextureAtlas::TextureAtlas(const std::string& name, const std::string& path, unsigned int sliceIndex)
		: m_Name(name), m_Index(sliceIndex)
	{
		LT_PROFILE_FUNC();

		std::string data = FileManager::LoadTextFile(path);

		std::stringstream stream(data);
		std::string line;

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

			m_Segments[name] = { xMin, yMin, xMax, yMax, static_cast<float>(m_Index) };
		}
	}

	std::shared_ptr<TextureArray> TextureArray::Create(unsigned int slices, unsigned int channels /* = 4 */)
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glTextureArray>(slices, channels);
		case GraphicsAPI::Directx:LT_DX(
			return std::make_shared<dxTextureArray>(slices, channels);)
		default:
			LT_CORE_ASSERT(false, "invalid GraphicsAPI");
		}
		return nullptr;
	}

}	