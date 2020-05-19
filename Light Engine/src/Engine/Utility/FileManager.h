#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Light {

	class FileManager
	{
	public:
		FileManager() = delete;

		static std::string LoadTextFile(const std::string& path);
		static TextureImageData LoadTextureFile(const std::string& path);
	};

}