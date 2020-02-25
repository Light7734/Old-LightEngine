#pragma once

#include "Core/Core.h"

#include <chrono>

namespace Light {

	using namespace std;

	class Time
	{
	private:
		static chrono::time_point<std::chrono::steady_clock> s_AppStartPoint;

		static float s_DeltaTime;
		static float s_PrevFrame;
		static float s_CurrentFrame;
	public:
		Time() = delete;

		static void CalculateDeltaTime();

		static inline float GetDeltaTime() { return s_DeltaTime; }
		static inline float ElapsedTime () { return ((chrono::duration<float>)(chrono::steady_clock::now() - s_AppStartPoint)).count(); }
	};

	class Timer
	{
	private:
		chrono::time_point< chrono::steady_clock > m_StartPoint;
	public:
		Timer() { Reset(); }
		~Timer() = default;

		inline void Reset() { m_StartPoint = chrono::steady_clock::now(); }
		
		inline float ElapsedTime() const
			{ return ((chrono::duration<float>)(chrono::steady_clock::now() - m_StartPoint)).count(); }
	};

}