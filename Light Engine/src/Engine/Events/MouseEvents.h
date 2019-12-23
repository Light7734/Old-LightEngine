#pragma once
#include "ltpch.h"

#include "Event.h"

#include "Core/Core.h"

#include <sstream>

namespace Light {

	class MouseMovedEvent : public Event
	{
	private:
		int16_t m_MouseX, m_MouseY;
	public:
		MouseMovedEvent(int16_t x, int16_t y): m_MouseX(x), m_MouseY(y) {}

		inline uint16_t GetX() { return m_MouseX; }
		inline uint16_t GetY() { return m_MouseY; }

		inline std::pair<uint16_t, uint16_t> GetPos() { return { m_MouseX, m_MouseY }; }

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
		uint8_t m_Button;
	public:
		MouseButtonPressedEvent(uint8_t button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override
		{
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button pressed: " << (int)m_Button;
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPressed)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseButtonReleasedEvent : public Event
	{
	private:
		uint8_t m_Button;
	public:
		MouseButtonReleasedEvent(uint8_t button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override 
		{ 
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button released: " << (int)m_Button;
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