#include "ltpch.h"
#include "Instrumentor.h"

namespace Light {

	Instrumentor::Instrumentor()
		: m_ProfileCount(0)
	{
	}

	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

	void Instrumentor::BeginSession(const std::string& filepath /* = "results.json" */)
	{
		m_OutputStream.open(filepath);
		WriteHeader();
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();
		m_OutputStream.close();
		m_ProfileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.end - result.start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.threadID << ",";
		m_OutputStream << "\"ts\":" << result.start;
		m_OutputStream << "}";
	}

	void Instrumentor::WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
	}

	void Instrumentor::WriteFooter()
	{
		m_OutputStream << "]}";
	}

	InstrumentationTimer::InstrumentationTimer(const char* name)
		: m_Result({name, 0, 0, 0})
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		m_Result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		m_Result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		Instrumentor::Get().WriteProfile(m_Result);
	}

}