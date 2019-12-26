#include "ltpch.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

#if   defined(LIGHT_PLATFORM_WINDOWS)
	#define GLFW_EXPOSE_NATIVE_WIN32

#elif defined(LIGHT_PLATFORM_LINUX)
	#define GLFW_EXPOSE_NATIVE_X11 // #todo: is this the correct one?

#elif defined (LIGHT_PLATFORM_MAC)
	#define GLFW_EXPOSE_NATIVE_COCOA

#endif
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

namespace Light {

	Window::Window(const WindowData& data)
	{
		m_Data = data;
		glfwInit();

		m_Window = glfwCreateWindow(data.width, data.height, data.title.c_str(),
		                            data.displayMode == DisplayMode::ExclusiveFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		Center();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::HandleEvents()
	{
		glfwPollEvents();
	}

	void Window::SetEventCallbackFunction(std::function<void(Event&)> event_callback_func)
	{
		m_Data.eventCallback = event_callback_func;
		SetGlfwCallbacks();
	}

	void Window::Resize(uint16_t width, uint16_t height)
	{
		glfwSetWindowSize(m_Window, width, height);
	}

	void Window::Reposition(int16_t x, int16_t y)
	{
		glfwSetWindowPos(m_Window, x, y);
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.title = title;
	}

	void Window::SetWindowState(WindowState state)
	{
		switch (state)
		{
		case Light::WindowState::Visible:
			glfwShowWindow(m_Window);
			break;

		case Light::WindowState::Hidden:
			glfwHideWindow(m_Window);
			break;

		case Light::WindowState::Minimized:
		{
			glfwIconifyWindow(m_Window);

			WindowMinimizedEvent event;
			m_Data.eventCallback(event);

			break;
		}
		default:
			LT_CORE_ERROR("Invalid window style");
		}

		m_Data.state = state;
	}	

	void Window::SetDisplayMode(DisplayMode mode)
	{
		static unsigned int backupWidth, backupHeight;

		switch (mode)
		{
		case DisplayMode::ExclusiveFullscreen:
		{
			backupWidth  = m_Data.width;
			backupHeight = m_Data.height;

			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, 0);

			m_Data.width = vidMode->width;
			m_Data.height = vidMode->height;

			WindowMaximizedEvent event;
			m_Data.eventCallback(event);

			break;
		}

		case DisplayMode::Windowed: case DisplayMode::BorderlessWindowed:
		{
			if (m_Data.displayMode == DisplayMode::ExclusiveFullscreen)
			{
				m_Data.width = backupWidth;
				m_Data.height = backupHeight;
			}

			glfwSetWindowMonitor(m_Window, nullptr, m_Data.x, m_Data.y, m_Data.width, m_Data.height, 0);
			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, mode == DisplayMode::Windowed);

			WindowRestoredEvent event;
			m_Data.eventCallback(event);

			break;
		}
		default:
			LT_CORE_ERROR("Invalid display mode");
		}

		m_Data.displayMode = mode;
		Center();
	}

	void Window::Center()
	{
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);

		int monitorX, monitorY;
		int windowWidth, windowHeight;

		glfwGetMonitorPos(monitor , &monitorX   , &monitorY    );
		glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

		m_Data.x = monitorX + (mode->width  - windowWidth ) / 2;
		m_Data.y = monitorY + (mode->height - windowHeight) / 2;

		glfwSetWindowPos(m_Window, m_Data.x, m_Data.y);
	}

	void Window::Close()
	{
		m_Data.running = false;
	}

	void* Window::GetNativeHandle() const
	{
#ifdef GLFW_EXPOSE_NATIVE_WIN32
		return glfwGetWin32Window(m_Window);

#elif
#error "Light engine only supports window for now"
#endif
	}

	void Window::SetGlfwCallbacks()
	{
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(button);
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(event);
			}
			else
			{
				MouseButtonReleasedEvent event(button);
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(event);
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			MouseMovedEvent event(xpos, ypos);
			(*(WindowData*)glfwGetWindowUserPointer(window)).eventCallback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			MouseScrolledEvent event(yoffset);
			(*(WindowData*)glfwGetWindowUserPointer(window)).eventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS /* || action == GLFW_REPEAT */)
			{
				KeyboardKeyPressedEvent event(key);
				(*(WindowData*)glfwGetWindowUserPointer(window)).eventCallback(event);
			}
			if (action == GLFW_RELEASE)
			{
				KeyboardKeyReleasedEvent event(key);
				(*(WindowData*)glfwGetWindowUserPointer(window)).eventCallback(event);
			}
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			auto data = ((WindowData*)glfwGetWindowUserPointer(window));
			WindowResizedEvent event(width, height);

			data->width = width;
			data->height = height;
			data->eventCallback(event);

			glViewport(0, 0, width, height);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event(xpos, ypos);

			data->x = xpos;
			data->y = ypos;
			data->eventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);

			if (focused)
			{
				WindowFocusedEvent event;

				data->active = true;
				data->eventCallback(event);
			}
			else
			{
				WindowLostFocusEvent event;

				data->active = false;
				data->eventCallback(event);
			}
		});

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconify) 
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);
			if (iconify)
			{
				WindowMinimizedEvent event;
				data->state = WindowState::Minimized;
				data->eventCallback(event);
			}
			else
			{
				WindowRestoredEvent  event;
				data->state = WindowState::Visible;
				data->eventCallback(event);
			}
		});

		glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximize) 
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);

			if (maximize)
			{
				WindowMaximizedEvent event;
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(event);
			}
			else
			{
				WindowRestoredEvent  event;
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(event);
			}
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowClosedEvent event;
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(event);
		});

	}

}