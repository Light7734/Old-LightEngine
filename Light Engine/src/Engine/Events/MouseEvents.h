#pragma once

#include "Event.h"

#include "Debug/InputcodeToString.h"

#include "Core/Core.h"

#include <sstream>

namespace Light {

	class MouseMovedEvent : public Event
	{
	private:
		int m_MouseX, m_MouseY;
	public:
		MouseMovedEvent(int x, int y): m_MouseX(x), m_MouseY(y) {}

		inline int GetX() { return m_MouseX; }
		inline int GetY() { return m_MouseY; }

		inline std::pair<int, int> GetPos() { return { m_MouseX, m_MouseY }; }

		std::string GetLogInfo() const override
		{
			std::stringstream ss;
			ss << "Mouse moved: [ " << m_MouseX << ", " << m_MouseY << " ]";
			return ss.str();
		}

		EVENT_TYPE(MouseMoved)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseButtonPressedEvent : public Event
	{
	private:
		int m_Button;
	public:
		MouseButtonPressedEvent(int button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override
		{
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button pressed: " << ButtonToString(m_Button);
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPressed)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseButtonReleasedEvent : public Event
	{
	private:
		int m_Button;
	public:
		MouseButtonReleasedEvent(uint8_t button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override 
		{ 
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button released: " << ButtonToString(m_Button);
			return  ss.str();
		}

		EVENT_TYPE(MouseButtonReleased)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseScrolledEvent : public Event
	{
	private:
		int8_t m_XOffset;
	public:
		MouseScrolledEvent(int8_t xOffset): m_XOffset(xOffset) {}

		inline int8_t GetOffset() { return m_XOffset; }

		std::string GetLogInfo() const override 
		{
			std::stringstream ss;
			ss << "Mouse scrolled: " << (int)m_XOffset;
			return ss.str();
		}

		EVENT_TYPE(MouseScrolled)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

}