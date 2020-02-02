#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Light {

	class FileManager
	{
	public:
		FileManager() = delete;

		static std::string LoadTextFile(const std::string& path);

		static std::shared_ptr<TextureData> LoadTextureFile(const std::string& path, bool flipped);
	};

}