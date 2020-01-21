#include "ltpch.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#if   defined(LIGHT_PLATFORM_WINDOWS)
	#define GLFW_EXPOSE_NATIVE_WIN32

#elif defined(LIGHT_PLATFORM_LINUX)
	#define GLFW_EXPOSE_NATIVE_X11 // #todo: is this the correct one?

#elif defined (LIGHT_PLATFORM_MAC)
	#define GLFW_EXPOSE_NATIVE_COCOA

#endif

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

namespace Light {

	Window* Window::s_Instance = nullptr;
	WindowData Window::s_Data = {};

	GLFWwindow* Window::s_GlfwHandle = nullptr;
	void*       Window::s_NativeHandle = nullptr;

	Window::Window(const WindowData& data)
	{
		LT_CORE_ASSERT(!s_Instance, "Multiple Window instances");
		LT_CORE_ASSERT(glfwInit(), "Failed to intialize glfw");

		s_Instance = this;
		s_Data = data;


		// Window hints
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE  , data.visible);
		glfwWindowHint(GLFW_DECORATED, data.displayMode != DisplayMode::BorderlessWindowed);


		// Create and check glfw window
		s_GlfwHandle = glfwCreateWindow(data.width, data.height, data.title.c_str(), 
		                                data.displayMode == DisplayMode::Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		                                nullptr);
		LT_CORE_ASSERT(s_GlfwHandle, "Failed to create glfw window");


		glfwSetWindowUserPointer(s_GlfwHandle, &s_Data);
		Center();

		// Set native window handle
#		ifdef GLFW_EXPOSE_NATIVE_WIN32
			s_NativeHandle = glfwGetWin32Window(s_GlfwHandle);
#		else
			#error "Light engine only supports Windows for now"
#		endif
	}

	Window::~Window()
	{
		glfwDestroyWindow(s_GlfwHandle);
		glfwTerminate();
	}

	void Window::HandleEvents()
	{
		glfwPollEvents();
	}

	void Window::SetEventCallbackFunction(std::function<void(Event&)> event_callback_func)
	{
		s_Data.eventCallback = event_callback_func;
		s_Instance->SetGlfwCallbacks();
	}

	void Window::Resize(unsigned int width, unsigned int height)
	{
		glfwSetWindowSize(s_GlfwHandle, width, height);
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(s_GlfwHandle, title.c_str());
		s_Data.title = title;
	}

	void Window::SetDisplayMode(DisplayMode mode)
	{
		static unsigned int backupWidth, backupHeight;

		switch (mode)
		{
		case DisplayMode::Fullscreen:
		{
			if (s_Data.displayMode != DisplayMode::Fullscreen)
			{
				backupWidth = s_Data.width;
				backupHeight = s_Data.height;
			}

			s_Data.eventCallback(WindowMaximizedEvent());

			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(s_GlfwHandle, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, 0);

			s_Data.width = vidMode->width;
			s_Data.height = vidMode->height;

			break;
		}

		case DisplayMode::Windowed: case DisplayMode::BorderlessWindowed:
		{
			if (s_Data.displayMode == DisplayMode::Fullscreen)
			{
				s_Data.width = backupWidth;
				s_Data.height = backupHeight;
			}

			s_Data.eventCallback(WindowRestoredEvent());

			glfwSetWindowMonitor(s_GlfwHandle, nullptr, 0, 0, s_Data.width, s_Data.height, 0);
			glfwSetWindowAttrib(s_GlfwHandle, GLFW_DECORATED, mode == DisplayMode::Windowed);
			Center();

			break;
		}
		default:
			LT_CORE_ERROR("Invalid display mode");
		}

		s_Data.displayMode = mode;
		Center();
	}

	void Window::SetVisibility(bool visible)
	{
		if (visible)
			glfwShowWindow(s_GlfwHandle);
		else
			glfwHideWindow(s_GlfwHandle);

		s_Data.visible = visible;
	}

	void Window::Center()	
	{
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);

		int monitorX, monitorY;
		int windowWidth, windowHeight;

		glfwGetMonitorPos(monitor,      &monitorX   , &monitorY    );
		glfwGetWindowSize(s_GlfwHandle, &windowWidth, &windowHeight);

		glfwSetWindowPos(s_GlfwHandle, monitorX + (mode->width  - windowWidth ) / 2,
		                               monitorY + (mode->height - windowHeight) / 2);
	}

	void Window::Close()
	{
		s_Data.closed = true;
	}

	void Window::SetGlfwCallbacks()
	{
		glfwSetErrorCallback([](int code, const char* error) 
		{
			// #todo: Improve:
			LT_CORE_ERROR(error);
		});

		glfwSetMouseButtonCallback(s_GlfwHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseButtonPressedEvent(button));
			else
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseButtonReleasedEvent(button));
		});

		glfwSetCursorPosCallback(s_GlfwHandle, [](GLFWwindow* window, double xpos, double ypos)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseMovedEvent(static_cast<int>(xpos), static_cast<int>(ypos)));
		});

		glfwSetScrollCallback(s_GlfwHandle, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseScrolledEvent(static_cast<int>(yoffset)));
		});

		glfwSetKeyCallback(s_GlfwHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(KeyboardKeyPressedEvent(key));
			if (action == GLFW_RELEASE)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(KeyboardKeyReleasedEvent(key));
		});

		glfwSetWindowSizeCallback(s_GlfwHandle, [](GLFWwindow* window, int width, int height)
		{
			auto data = ((WindowData*)glfwGetWindowUserPointer(window));

			data->width = width;
			data->height = height;
			data->eventCallback(WindowResizedEvent(width, height));
		});

		glfwSetWindowPosCallback(s_GlfwHandle, [](GLFWwindow* window, int xpos, int ypos)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowMovedEvent(xpos, ypos));
		});

		glfwSetWindowFocusCallback(s_GlfwHandle, [](GLFWwindow* window, int focused)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);

			if (focused)
			{
				data->active = true;
				data->eventCallback(WindowFocusedEvent());
			}
			else
			{
				data->active = false;
				data->eventCallback(WindowLostFocusEvent());
			}
		});

		glfwSetWindowIconifyCallback(s_GlfwHandle, [](GLFWwindow* window, int iconify)
		{
			if (iconify)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowMinimizedEvent());
			else
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowRestoredEvent());
		});

		glfwSetWindowMaximizeCallback(s_GlfwHandle, [](GLFWwindow* window, int maximize)
		{
			if (maximize)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowMaximizedEvent());
			else
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowRestoredEvent());
		});

		glfwSetWindowCloseCallback(s_GlfwHandle, [](GLFWwindow* window)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowClosedEvent());
		});
	}

}