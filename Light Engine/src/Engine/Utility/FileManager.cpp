#include "ltpch.h"
#include "FIleManager.h"

#include "stb_image.h"

namespace Light {

	std::string FileManager::LoadTextFile(const std::string& path)
	{
		std::fstream stream(path);
		if (!stream.is_open())
		{
			LT_CORE_ERROR("FileManager::LoadTextFile: Failed to load text file: {}", path);
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
		unsigned char* pixels = stbi_load(path.c_str(), x, y, channels, 4);
		if (!pixels)
			LT_CORE_ERROR("FileManager::LoadTextureFile: Failed to load texture file: {}", path);
		return pixels;
	}

}