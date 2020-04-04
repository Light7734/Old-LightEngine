/*
* Thanks to @TheCherno for the gist, and @davechurchill for the edit
*/

#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>

namespace Light {

	struct ProfileResult
	{
		std::string name;
		long long start, end;
		uint32_t threadID;
	};

	class Instrumentor
	{
	private:
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	private:
		Instrumentor();
	public:
		static Instrumentor& Get();

		void BeginSession(const std::string& filepath = "results.json");
		void EndSession();

		void WriteProfile(const ProfileResult& result);
		void WriteHeader();
		void WriteFooter();
	};

	class InstrumentationTimer
	{
	private:
		ProfileResult m_Result;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	public:
		InstrumentationTimer(const char* name);
		~InstrumentationTimer();
	};

}
