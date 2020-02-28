#pragma once

#include "Core/Core.h"

namespace Light {

	class FileManager
	{
	public:
		FileManager() = delete;


		static std::string LoadTextFile(const std::string& path);
		static unsigned char* LoadTextureFile(const std::string& path, int* x, int* y, int* channels);
	};

}