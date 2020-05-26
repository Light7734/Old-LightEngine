#pragma once

#include "Debug/Logger.h"
#include "Debug/Benchmark/Instrumentor.h"


// platform
#ifdef _WIN32
	#define LIGHT_PLATFORM_WINDOWS
	#ifndef _WIN64
		#error x86 Architecture is not supported
	#endif
#elif defined(__LINUX__)
	#define LIGHT_PLATFORM_LINUX
	#error Unsupported platform: Linux
#elif defind (__APPLE__) || defined(__MACH__)
	#define LIGHT_PLATFORM_MAC
	#error Unsupported platform: Mac
#else
	#error Unsupported platform: Unknown
#endif


// directx
#ifdef LIGHT_PLATFORM_WINDOWS
	#define LT_DX(x) x
#else
	#define LT_DX(x)
#endif

// benchmark
#if !defined(LIGHT_DIST) && !defined(DISABLE_LIGHT_BENCHMARKING)
	#define LT_PROFILE_SCOPE(name) ::Light::InstrumentationTimer timer##__LINE__(name) 
	#define LT_PROFILE_FUNC() LT_PROFILE_SCOPE(__FUNCSIG__)

	#define LT_PROFILE_SESSION_BEGIN(filePath) ::Light::Instrumentor::Get().BeginSession(filePath) 
	#define LT_PROFILE_SESSION_END() ::Light::Instrumentor::Get().EndSession()
#else
	#define LT_PROFILE_SCOPE(name)
	#define LT_PROFILE_FUNC()

	#define LT_PROFILE_SESSION_BEGIN(filePath)
	#define LT_PROFILE_SESSION_END()
#endif

// events
#define LT_EVENT_FN(fn)        std::bind(&##fn, this, std::placeholders::_1)
#define LT_EVENT_FN_STATIC(fn) std::bind(&##fn,       std::placeholders::_1)

// break
#ifndef LIGHT_DIST
	#define LT_DBREAK __debugbreak()
#else
	#define LT_DBREAK
#endif

// assertion
#define LT_ASSERT(x, ...)      if(!(x)) { LT_FATAL(__VA_ARGS__)     ; LT_DBREAK; throw ::Light::FailedAssertion(__FILE__, __LINE__); }
#define LT_CORE_ASSERT(x, ...) if(!(x)) { LT_CORE_FATAL(__VA_ARGS__); LT_DBREAK; throw ::Light::FailedAssertion(__FILE__, __LINE__); }


// utility
#define LT_MAP_MACRO_VALUE_TO_NAME(macro) { macro, #macro }
#define LT_MAP_MACRO_NAME_TO_VALUE(macro) { #macro, macro }
#define BIT(x) 1 << x


namespace Light {

	constexpr unsigned int hashStr(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (hashStr(str, h + 1) * 33) ^ str[h];
	}

	struct FailedAssertion : public std::exception 
	{
		FailedAssertion(const char* file, int line)		
		{
			LT_CORE_FATAL("assertion failed, File: {}, Line: {}", file, line);
		}
	};

}