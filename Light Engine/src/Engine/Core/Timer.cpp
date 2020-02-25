#include "ltpch.h"
#include "Timer.h"

namespace Light {

	std::chrono::time_point<std::chrono::steady_clock> Time::s_AppStartPoint = std::chrono::steady_clock::now();

	float Time::s_DeltaTime    = 0.0f;
	float Time::s_CurrentFrame = 0.0f;
	float Time::s_PrevFrame    = 0.0f;

	void Time::CalculateDeltaTime()
	{
		s_CurrentFrame = ElapsedTime();
		s_DeltaTime = s_CurrentFrame - s_PrevFrame;
		s_PrevFrame = s_CurrentFrame;
	}

}