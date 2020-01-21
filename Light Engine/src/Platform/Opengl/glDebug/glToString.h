#pragma once

#include "Core/Core.h"

namespace Light {

	class glToString
	{
	public:
		glToString() = delete;

		static std::string DebugMsgSeverity(unsigned int severity);
		static std::string DebugMsgSource(unsigned int source);
		static std::string DebugMsgType(unsigned int type);
	};

}