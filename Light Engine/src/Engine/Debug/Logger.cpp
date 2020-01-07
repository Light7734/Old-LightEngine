#include "ltpch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
#include <time.h>

namespace Light {

	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_GameLogger;
	std::shared_ptr<spdlog::logger> Logger::s_FileLogger;

	std::string Logger::s_FileLogPath = "";
	bool Logger::s_Initialized = false;

	void Logger::Init()
	{
		if (s_Initialized)
			{ LT_CORE_WARN("Logger::Init() called multiple times"); return; }


		// spdlog doesn't create a folder, so we need to make sure it exists
		std::filesystem::create_directory("Logs");

		// Set output directory
		InitLogFileOutputDir();

		// Initialize spdlog
		spdlog::set_level(spdlog::level::trace);

		s_CoreLogger = spdlog::stdout_color_mt("<Engine>");
		s_GameLogger = spdlog::stdout_color_mt("< Game >");
		s_FileLogger = spdlog::basic_logger_mt("< File >", s_FileLogPath);

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_FileLogger->set_pattern("[%T]: <%l>: %v");


		s_Initialized = true;
	}

	void Logger::Terminate()
	{
		if (!s_Initialized)
		{ 
#ifndef LIGHT_DIST
			printf("WARNING: Logger::Terminate() called (without a call to Logger::Init() / multiple times)\n");
#else
			if (s_FileLogPath.empty())
				InitLogFileOutputDir();

			std::ofstream out;
			out.open(s_FileLogPath, std::ios::app);
			out << "WARNING: Logger::Terminate() called (without a call to Logger::Init() / multiple times)\n";
#endif
			return; 
		}

		LT_CORE_DEBUG("Terminating Logger");

		s_CoreLogger.reset();
		s_GameLogger.reset();
		s_FileLogger.reset();

		spdlog::shutdown();

		s_Initialized = false;
	}

	void Logger::InitLogFileOutputDir()
	{
		// log file's name is the time logger reached its constructor
		time_t timeAndDate = time(0);
		s_FileLogPath = ctime(&timeAndDate);
		s_FileLogPath = s_FileLogPath.substr(3, s_FileLogPath.size() - 4);
		std::replace(s_FileLogPath.begin(), s_FileLogPath.end(), ':', '.');
		s_FileLogPath = "Logs/" + s_FileLogPath + ".log";

	}

}
