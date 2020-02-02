#include "ltpch.h"
#include "FIleManager.h"

#include "stb_image.h"

namespace Light {

	std::string FileManager::LoadTextFile(const std::string& path)
	{
		std::fstream stream(path);
		if (!stream.is_open())
		{
			LT_CORE_ERROR("Failed to load text file: {}", path);
			return "";
		}

		std::string line;
		std::stringstream ss;
		while (std::getline(stream, line))
			ss << line << '\n';

		return ss.str();
	}

	std::shared_ptr<TextureData> FileManager::LoadTextureFile(const std::string& path, bool flipped)
	{
		unsigned char* pixels = nullptr;
		int width = 0, height = 0, channels = 0;

		stbi_set_flip_vertically_on_load(flipped);
		pixels = stbi_load(path.c_str(), &width, &height, &channels, NULL);

		std::shared_ptr<TextureData> data = TextureData::Create(pixels, width, height, channels);

		if (!pixels)
			LT_CORE_ERROR("Failed to load image file: {}", path);

		return data;
	}

}