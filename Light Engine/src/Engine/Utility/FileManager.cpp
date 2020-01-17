#include "ltpch.h"
#include "FIleManager.h"

namespace Light {

	std::string FileManager::ReadTxtFile(const std::string& path)
	{
		std::fstream stream(path);

		if (!stream.is_open())
		{
			LT_CORE_ERROR("Failed to read txt file: {}", path);
			return "";
		}

		
		std::string line;
		std::stringstream ss;
		while (std::getline(stream, line))
			ss << line << '\n';
			

		return ss.str();
	}

}