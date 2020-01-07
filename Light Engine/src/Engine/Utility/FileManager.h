#pragma once

#include "Core/Core.h"

namespace Light {

	class FileManager
	{
	public:
		FileManager() = delete;
		static std::string ReadTxtFile(const std::string& path);
	};

}