#include "ltpch.h"
#include "Window.h"

#include "Monitor.h"

#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include "Utility/FileManager.h"
#include "Renderer/Texture.h"

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

	Window* Window::s_Context = nullptr;

	Window::Window(const WindowData& data, const GraphicsConfigurations& configurations, GraphicsAPI api)
		: m_Data(data)
	{
		LT_PROFILE_FUNC();

		LT_CORE_ASSERT(!s_Context, "Window::Window: multiple Window instances");
		LT_CORE_ASSERT(glfwInit(), "Window::Window: call to glfwInit() failed");
		s_Context = this;

		// window hints
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE  , data.visible);
		glfwWindowHint(GLFW_DECORATED, data.displayMode != DisplayMode::BorderlessWindowed);
		
		// create and check glfw window
		m_GlfwHandle = glfwCreateWindow(configurations.resolution.width, configurations.resolution.height,
		                                data.title.c_str(),	
		                                data.displayMode == DisplayMode::Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		                                nullptr);
		LT_CORE_ASSERT(m_GlfwHandle, "Window::Window: call to glfwCreateWindow() failed");

		glfwSetWindowUserPointer(m_GlfwHandle, &m_Data);

		Center();
		if(data.minimized)
			glfwIconifyWindow(m_GlfwHandle);

		// set native window handle
#		ifdef GLFW_EXPOSE_NATIVE_WIN32
			m_NativeHandle = glfwGetWin32Window(m_GlfwHandle);
#		else
			#error Light engine only supports Windows for now
#		endif

		// initialize window dependent classes
		Monitor::Init();
		GraphicsContext::CreateContext(api, configurations);
	}

	Window::~Window()
	{
		LT_PROFILE_FUNC();

		glfwDestroyWindow(m_GlfwHandle);
		glfwTerminate();
	}

	void Window::HandleEvents()
	{
		glfwPollEvents();
	}

	void Window::SetEventCallbackFunction(std::function<void(Event&)> event_callback_func)
	{
		LT_PROFILE_FUNC();

		m_Data.eventCallback = event_callback_func;
		SetGlfwCallbacks();
	}

	void Window::SetMouseCursor(const std::string& texturePath, unsigned int hotspotX, unsigned int hotspotY)
	{
		LT_PROFILE_FUNC();

		glfwDestroyCursor(m_Data.cursor);

		TextureImageData tdata = FileManager::LoadTextureFile(texturePath);

		LT_CORE_ASSERT(tdata, "Window::SetMouseCursor: failed to load cursor texture: {}", texturePath);
		LT_CORE_ASSERT(hotspotX <= tdata.width && hotspotY <= tdata.height, "Window::SetMouseCursor: invalid hotspot position");

		GLFWimage image = {tdata.width, tdata.height, tdata.pixels};
		m_Data.cursor = glfwCreateCursor(&image, hotspotX, hotspotY);
		glfwSetCursor(m_GlfwHandle, m_Data.cursor);
	}

	void Window::SetTitle(const std::string& title)
	{
		LT_PROFILE_FUNC();

		glfwSetWindowTitle(m_GlfwHandle, title.c_str());
		m_Data.title = title;
	}

	void Window::SetDisplayMode(DisplayMode mode)
	{
		LT_PROFILE_FUNC();

		switch (mode)
		{
		case DisplayMode::Fullscreen:
		{
			glfwSetWindowAttrib(m_GlfwHandle, GLFW_DECORATED, true);

			glfwSetWindowMonitor(m_GlfwHandle, glfwGetPrimaryMonitor(), 0, 0,
			                     GraphicsContext::GetConfigurations().resolution.width,
			                     GraphicsContext::GetConfigurations().resolution.height, 0);

			m_Data.eventCallback(WindowRestoredEvent());
			break;
		}

		case DisplayMode::Windowed: case DisplayMode::BorderlessWindowed:
		{

			glfwSetWindowAttrib(m_GlfwHandle, GLFW_DECORATED, mode == DisplayMode::Windowed);

			glfwSetWindowMonitor(m_GlfwHandle,
			                     mode == DisplayMode::Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			                     0, 0,
			                     GraphicsContext::GetConfigurations().resolution.width,
			                     GraphicsContext::GetConfigurations().resolution.height, 0);

			m_Data.eventCallback(WindowRestoredEvent());
			break;
		}
		default:
			LT_CORE_ERROR("Window::SetDisplayMode: invalid display mode: {}", mode);
		}

		Center();
		m_Data.displayMode = mode;
	}

	void Window::SetVisibility(bool visible)
	{
		LT_PROFILE_FUNC();

		if (visible)
			glfwShowWindow(m_GlfwHandle);
		else
			glfwHideWindow(m_GlfwHandle);

		m_Data.visible = visible;
	}

	void Window::Center()	
	{
		LT_PROFILE_FUNC();

		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);

		int monitorX, monitorY;
		int windowWidth, windowHeight;

		glfwGetMonitorPos(monitor,                  &monitorX   , &monitorY    );
		glfwGetWindowSize(m_GlfwHandle, &windowWidth, &windowHeight);

		glfwSetWindowPos(m_GlfwHandle, monitorX + (mode->width  - windowWidth ) / 2,
		                                           monitorY + (mode->height - windowHeight) / 2);
	}

	void Window::Close()
	{
		m_Data.closed = true;
	}

	void Window::SetGlfwCallbacks()
	{
		LT_PROFILE_FUNC();

		glfwSetErrorCallback([](int code, const char* error) 
		{
			// #todo: improve:
			LT_CORE_ERROR("glfwErrorCallback: code: {}, desc: {}", code, error);
		});

		glfwSetMouseButtonCallback(m_GlfwHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseButtonPressedEvent(button));
			else
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseButtonReleasedEvent(button));
		});

		glfwSetCursorPosCallback(m_GlfwHandle, [](GLFWwindow* window, double xpos, double ypos)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseMovedEvent(glm::ivec2(xpos, ypos)));
		});

		glfwSetScrollCallback(m_GlfwHandle, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(MouseScrolledEvent(static_cast<int>(yoffset)));
		});

		glfwSetKeyCallback(m_GlfwHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(KeyboardKeyPressedEvent(key));
			if (action == GLFW_RELEASE)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(KeyboardKeyReleasedEvent(key));
		});

		glfwSetWindowSizeCallback(m_GlfwHandle, [](GLFWwindow* window, int width, int height)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowResizedEvent(glm::ivec2(width, height)));
		});

		glfwSetWindowPosCallback(m_GlfwHandle, [](GLFWwindow* window, int xpos, int ypos)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowMovedEvent(glm::ivec2(xpos, ypos)));
		});

		glfwSetWindowFocusCallback(m_GlfwHandle, [](GLFWwindow* window, int focused)
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

		glfwSetWindowIconifyCallback(m_GlfwHandle, [](GLFWwindow* window, int iconify)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);
			data->minimized = iconify;

			if (iconify)
				data->eventCallback(WindowMinimizedEvent());
			else
				data->eventCallback(WindowRestoredEvent());
		});

		glfwSetWindowMaximizeCallback(m_GlfwHandle, [](GLFWwindow* window, int maximize)
		{
			if (maximize)
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowMaximizedEvent());
			else
				((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowRestoredEvent());
		});

		glfwSetWindowCloseCallback(m_GlfwHandle, [](GLFWwindow* window)
		{
			((WindowData*)glfwGetWindowUserPointer(window))->eventCallback(WindowClosedEvent());
		});

	}

}