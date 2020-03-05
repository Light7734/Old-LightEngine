#pragma once

#include "Event.h"

#include "Core/Core.h"

#include "Input/Input.h"

#include <glm/glm.hpp>

#include <sstream>

namespace Light {

	class MouseMovedEvent : public Event
	{
	private:
		const glm::ivec2 m_MousePos;
	public:
		MouseMovedEvent(const glm::ivec2& position): m_MousePos(position){}

		inline int GetX() { return m_MousePos.x; }
		inline int GetY() { return m_MousePos.y; }

		inline const glm::ivec2& GetPos() { return m_MousePos; }

		std::string GetLogInfo() const override
		{
			std::stringstream ss;
			ss << "Mouse moved: [ " << m_MousePos.x << ", " << m_MousePos.y << " ]";
			return ss.str();
		}

		EVENT_TYPE(MouseMoved)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseButtonPressedEvent : public Event
	{
	private:
		const int m_Button;
	public:
		MouseButtonPressedEvent(int button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override
		{
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button pressed: " << Input::GetButtonName(m_Button);
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPressed)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseButtonReleasedEvent : public Event
	{
	private:
		const int m_Button;
	public:
		MouseButtonReleasedEvent(uint8_t button): m_Button(button) {}

		inline int GetButton() { return m_Button; }

		std::string GetLogInfo() const override 
		{ 
			// #todo: convert button's code to button's name
			std::stringstream ss;
			ss << "Mouse button released: " << Input::GetButtonName(m_Button);
			return  ss.str();
		}

		EVENT_TYPE(MouseButtonReleased)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

	class MouseScrolledEvent : public Event
	{
	private:
		const int m_XOffset;
	public:
		MouseScrolledEvent(int xOffset): m_XOffset(xOffset) {}

		inline int GetOffset() { return m_XOffset; }

		std::string GetLogInfo() const override 
		{
			std::stringstream ss;
			ss << "Mouse scrolled: " << m_XOffset;
			return ss.str();
		}

		EVENT_TYPE(MouseScrolled)
		EVENT_CATEGORY(EventCategory_Input | EventCategory_Mouse)
	};

}