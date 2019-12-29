#pragma once

#include <spdlog/spdlog.h>

#define LT_FILE_TRACE(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->trace   (__VA_ARGS__)
#define LT_FILE_DEBUG(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->debug   (__VA_ARGS__)
#define LT_FILE_INFO(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->info    (__VA_ARGS__)
#define LT_FILE_WARN(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->warn    (__VA_ARGS__)
#define LT_FILE_ERROR(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->error   (__VA_ARGS__)
#define LT_FILE_FATAL(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetFileLogger()->critical(__VA_ARGS__)

#ifndef LIGHT_DIST
	#define LT_TRACE(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->trace   (__VA_ARGS__)
	#define LT_DEBUG(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->debug   (__VA_ARGS__)
	#define LT_INFO(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->info    (__VA_ARGS__)
	#define LT_WARN(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->warn    (__VA_ARGS__)
	#define LT_ERROR(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->error   (__VA_ARGS__)
	#define LT_FATAL(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetGameLogger()->critical(__VA_ARGS__)
	
	#define LT_CORE_TRACE(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->trace   (__VA_ARGS__)
	#define LT_CORE_DEBUG(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->debug   (__VA_ARGS__)
	#define LT_CORE_INFO(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->info    (__VA_ARGS__)
	#define LT_CORE_WARN(...)  if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->warn    (__VA_ARGS__)
	#define LT_CORE_ERROR(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->error   (__VA_ARGS__)
	#define LT_CORE_FATAL(...) if(::Light::Logger::isInitialized()) ::Light::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#else
	#define LT_TRACE(...)
	#define LT_DEBUG(...)
	#define LT_INFO(...)
	#define LT_WARN(...)  LT_FILE_WARN (__VA_ARGS__)
	#define LT_ERROR(...) LT_FILE_ERROR(__VA_ARGS__)
	#define LT_FATAL(...) LT_FILE_FATAL(__VA_ARGS__)
	
	#define LT_CORE_TRACE(...)
	#define LT_CORE_DEBUG(...)
	#define LT_CORE_INFO(...)
	#define LT_CORE_WARN(...)  LT_FILE_WARN (__VA_ARGS__)
	#define LT_CORE_ERROR(...) LT_FILE_ERROR(__VA_ARGS__)
	#define LT_CORE_FATAL(...) LT_FILE_FATAL(__VA_ARGS__)
#endif

namespace Light {

	class Logger
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_GameLogger;
		static std::shared_ptr<spdlog::logger> s_FileLogger;

		static std::string s_FileLogPath;

		static bool s_Initialized;
	public:
		Logger() = delete;

		static void Init();
		static void Terminate();

		static inline void SetPattern(const char* pattern) { s_GameLogger->set_pattern(pattern); }

		static inline void SetCoreLevel(spdlog::level::level_enum lvl) { s_CoreLogger->set_level(lvl); }
		static inline void SetGameLevel(spdlog::level::level_enum lvl) { s_GameLogger->set_level(lvl); }

		static inline std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static inline std::shared_ptr<spdlog::logger> GetGameLogger() { return s_GameLogger; }
		static inline std::shared_ptr<spdlog::logger> GetFileLogger() { return s_FileLogger; }

		static inline bool isInitialized() { return s_Initialized; }
	private:
	};

}