#pragma once

#include "Event.h"

#include "Input/Input.h"

#include "Core/Core.h"

#include <sstream>

namespace Light {

	class KeyboardKeyPressedEvent : public Event
	{
	private:
		const int m_Key;
	public:
		KeyboardKeyPressedEvent(int16_t key): m_Key(key) {}

		inline int GetKey() const { return m_Key; }

		std::string GetLogInfo() const 
		{ 
			std::stringstream ss;
			ss << "KeyboardKeyPressed: " << Input::GetKeyName(m_Key);
			return ss.str();
		}

		EVENT_TYPE(KeyboardKeyPressed)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Keyboard)
	};

	class KeyboardKeyReleasedEvent : public Event
	{
	private:
		const int m_Key;
	public:
		KeyboardKeyReleasedEvent(int key): m_Key(key) {}

		inline int GetKey() const { return m_Key; }

		std::string GetLogInfo() const
		{ 
			std::stringstream ss;
			ss << "KeyboardkeyReleased: " << Input::GetKeyName(m_Key);
			return ss.str();
		}

		EVENT_TYPE(KeyboardKeyReleased)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Keyboard)
	};

}
