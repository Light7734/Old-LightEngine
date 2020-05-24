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

		float xRatio = texture.xMax / slice.xMax;
		float yRatio = texture.yMax / slice.yMax;

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

			m_SubTextures[name] = { xMin * xRatio, yMin * yRatio, xMax * xRatio, yMax * yRatio, texture.sliceIndex };
		}
	}

	Texture::Texture(const SubTexture& texture)
		: m_Texture(texture)
	{
	}

	TextureArray::TextureArray(unsigned int width, unsigned int height, unsigned int depth)
		: m_Width(width), m_Height(height), m_Depth(depth), m_CurrentIndex(0)
	{
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
	
	void TextureArray::CreateSlice(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		TextureImageData data = FileManager::LoadTextureFile(texturePath);
		unsigned int slice;

		if (!m_FreedSlices.empty())
		{
			slice = *m_FreedSlices.begin();
			m_FreedSlices.erase(slice);
		}
		else
			slice = m_CurrentIndex++;

		LT_CORE_ASSERT(data, "TextureArray::CreateSlice: failed to load texture file: {}", texturePath);
		LT_CORE_ASSERT(slice < m_Depth - 1, "TextureArray::CreateSlice: too many texture slices");

		UpdateSubTexture(0, 0, slice, data.width, data.height, data.pixels);
		GenerateMips();

		m_Textures[name] = std::make_shared<Texture>(atlasPath, SubTexture(0.0f, 0.0f, data.width, data.height, slice),
		                                             SubTexture(0.0f, 0.0f, m_Width, m_Height, slice));
	}

	void TextureArray::CreateSlice(const std::string& name, unsigned int width, unsigned int height)
	{
		unsigned int slice;

		if (!m_FreedSlices.empty())
		{
			slice = *m_FreedSlices.begin();
			m_FreedSlices.erase(slice);
		}
		else
			slice = m_CurrentIndex++;

		LT_CORE_ASSERT(slice < m_Depth - 1, "TextureArray::CreateSlice: too many texture slices");

		m_Textures[name] = std::make_shared<Texture>(SubTexture(0.0f, 0.0f, width, height, slice));
	}

	void TextureArray::DeleteSlice(const std::string& name)
	{
		m_FreedSlices.insert(m_Textures[name]->GetSliceIndex());
		m_Textures.erase(name);
	}
	
}	