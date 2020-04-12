#include "ltpch.h"
#include "FIleManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Light {

	std::string FileManager::LoadTextFile(const std::string& path)
	{
		LT_PROFILE_FUNC();

		std::fstream stream(path);
		if (!stream.is_open())
		{
			LT_CORE_ERROR("FileManager::LoadTextFile: failed to load text file: {}", path);
			return "";
		}

		std::string line;
		std::stringstream ss;
		while (std::getline(stream, line))
			ss << line << '\n';

		return ss.str();
	}

	unsigned char* FileManager::LoadTextureFile(const std::string& path, int* x, int* y, int* channels)
	{
		LT_PROFILE_FUNC();

		unsigned char* pixels = stbi_load(path.c_str(), x, y, channels, 4);
		if (!pixels)
			LT_CORE_ERROR("FileManager::LoadTextureFile: failed to load texture file: {}", path);
		return pixels;
	}

}