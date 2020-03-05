#pragma once

#include "Event.h"

#include "Core/Core.h"

#include <glm/glm.hpp>

#include <sstream>

namespace Light {

	class WindowResizedEvent : public Event
	{
	private:
		const glm::ivec2 m_Size;
	public:
		WindowResizedEvent(const glm::ivec2& size): m_Size(size) {}

		inline int GetWidth () const { return m_Size.x; }
		inline int GetHeight() const { return m_Size.y; }

		inline const glm::ivec2& GetSize() const { return m_Size; }

		std::string GetLogInfo() const override
		{
			std::stringstream ss;
			ss << "Window resized: [ " << m_Size.x << 'x' << m_Size.y << " ]";
			return ss.str();
		}

		EVENT_TYPE(WindowResized)
		EVENT_CATEGORY(EventCategory_Window)
	};

	class WindowMovedEvent : public Event
	{
	private:
		const glm::ivec2 m_WindowPos;
	public:
		WindowMovedEvent(const glm::ivec2& position): m_WindowPos(position) {}

		inline int GetX() const { return m_WindowPos.x; }
		inline int GetY() const { return m_WindowPos.y; }

		inline const glm::ivec2& GetPosition() const { return m_WindowPos; }

		std::string GetLogInfo() const override
		{ 
			std::stringstream ss;
			ss << "Window moved: [ " << m_WindowPos.x << ", " << m_WindowPos.y << " ]";
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