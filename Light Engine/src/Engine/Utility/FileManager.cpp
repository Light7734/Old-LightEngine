#include "ltpch.h"
#include "FIleManager.h"

namespace Light {

	std::string FileManager::ReadTxtFile(const std::string& path)
	{
		std::fstream stream(path);
		std::string line;
		std::stringstream ss;

		if (!stream.is_open())
		{
			LT_CORE_ERROR("Failed to read txt file: {}", path);
			return "";
		}

		while (std::getline(stream, line))
			ss << line << '\n';
			
		return ss.str();
	}

}