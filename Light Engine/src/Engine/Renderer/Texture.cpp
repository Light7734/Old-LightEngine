#include "ltpch.h"
#include "Texture.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxTexture.h"
#endif
#include "Platform/Opengl/glTexture.h"

#include "Utility/stb_image.h"

namespace Light {

	TextureData::~TextureData()
	{
		stbi_image_free(pixels);
	}

	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		std::shared_ptr<TextureData> data = FileManager::LoadTextureFile(path, false);

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glTexture>(data->pixels, data->width, data->height, data->channels);
		case GraphicsAPI::DirectX:
			return std::make_shared<dxTexture>(data->pixels, data->width, data->height, data->channels);
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}
	}

	std::shared_ptr<Texture> Texture::Create(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glTexture>(data, width, height, channels);
		case GraphicsAPI::DirectX:
			return std::make_shared<dxTexture>(data, width, height, channels);
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}
	}

}