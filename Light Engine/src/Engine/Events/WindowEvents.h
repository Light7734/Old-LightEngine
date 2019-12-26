#pragma once

#include "Event.h"

#include "Core/Core.h"

#include <sstream>

namespace Light {

	class WindowResizedEvent : public Event
	{
	private:
		int m_Width, m_Height;
	public:
		WindowResizedEvent(uint16_t  width, uint16_t  height): m_Width(width), m_Height(height) {}

		inline int GetWidth () const { return m_Width;  }
		inline int GetHeight() const { return m_Height; }

		inline std::pair<int, int> GetSize() const { return { m_Width, m_Height }; }

		std::string GetLogInfo() const override
		{
			std::stringstream ss;
			ss << "Window resized: [ " << m_Width << 'x' << m_Height << " ]";
			return ss.str();
		}

		EVENT_TYPE(WindowResized)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowMovedEvent : public Event
	{
	private:
		int m_PositionX, m_PositionY;
	public:
		WindowMovedEvent(int xpos, int ypos): m_PositionX(xpos), m_PositionY(ypos) {}

		inline int GetX() const { return m_PositionX; }
		inline int GetY() const { return m_PositionY; }

		inline std::pair<int, int> GetPosition() const { return { m_PositionX, m_PositionY }; }

		std::string GetLogInfo() const override
		{ 
			std::stringstream ss;
			ss << "Window moved: [ " << m_PositionX << ", " << m_PositionY << " ]";
			return ss.str();
		}

		EVENT_TYPE(WindowMoved)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowFocusedEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window gained input focus"; }

		EVENT_TYPE(WindowFocused)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window lost input focus"; }

		EVENT_TYPE(WindowLostFocus)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowRestoredEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window restored"; }

		EVENT_TYPE(WindowRestored)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowMaximizedEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window maximized"; }

		EVENT_TYPE(WindowMaximized)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowMinimizedEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window minimized"; }

		EVENT_TYPE(WindowMinimized)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowClosedEvent : public Event
	{
	public:
		std::string GetLogInfo() const override { return "Window should close"; }

		EVENT_TYPE(WindowClosed)
		EVENT_CATEGORY(EventCategory_Window)
	};

}