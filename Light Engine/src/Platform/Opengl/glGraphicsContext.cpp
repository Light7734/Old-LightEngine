#include "ltpch.h"
#include "glGraphicsContext.h"

#include "Core/Monitor.h"
#include "Core/Window.h"

#include "Debug/Exceptions.h"

#include "glDebug/glToString.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Light {

	glGraphicsContext::glGraphicsContext(const GraphicsConfigurations& configurations)
		: m_WindowHandle(Window::GetGlfwHandle())
	{
		LT_PROFILE_FUNC();

		m_Configurations = configurations;

		glfwMakeContextCurrent(m_WindowHandle);
		LT_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
		               "glGraphicsContext::glGraphicsContext: gladLoadGLLoader failed");

		// set configurations and debug callback
		SetConfigurations(configurations);
		SetDebugMessageCallback();

		// #todo: log more information
		LT_CORE_INFO("glGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", glGetString(GL_RENDERER));
		LT_CORE_INFO("        Version : {}", glGetString(GL_VERSION ));
	}

	void glGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void glGraphicsContext::ClearBackbuffer(float colors[4])
	{
		glClearColor(colors[0], colors[1], colors[2], colors[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void glGraphicsContext::Draw(unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void glGraphicsContext::DrawIndexed(unsigned int count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void glGraphicsContext::DefaultRenderBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void glGraphicsContext::SetConfigurations(const GraphicsConfigurations& configurations)
	{
		LT_PROFILE_FUNC();

		SetResolution(configurations.resolution);
		SetVSync(configurations.vSync);
	}
	
	void glGraphicsContext::SetResolution(const Resolution& resolution)
	{
		LT_PROFILE_FUNC();

		std::shared_ptr<Monitor> windowMonitor = Monitor::GetWindowMonitor();
		std::vector<VideoMode> videoModes = windowMonitor->GetVideoModes();

		// find maximum monitor resolution
		int maxWidth = 0, maxHeight = 0;
		for (const auto& videoMode : videoModes)
		{
			if (videoMode.width > maxWidth)
				maxWidth = videoMode.width;

			if (videoMode.height > maxHeight)
				maxHeight = videoMode.height;
		}

		// make sure the new resolution is not higher than monitor's
		if (resolution.width > maxWidth || resolution.height > maxHeight)
		{
			LT_CORE_ERROR("GraphicsContext::SetResolution: Window's resolution cannot be higher than monitor's: [{}x{}] > [{}x{}]",
			              resolution.width, resolution.height,
			              maxWidth , maxHeight);
			return;
		}

		// update configurations
		m_Configurations.resolution = resolution;

		// resize the Window and center it
		glfwSetWindowSize(m_WindowHandle, resolution.width, resolution.height);
		Window::Center();

		// adjust the viewport
		glViewport(0, 0, resolution.width, resolution.height);
	}

	void glGraphicsContext::SetVSync(bool vSync)
	{
		LT_PROFILE_FUNC();

		glfwSwapInterval(vSync);
		m_Configurations.vSync = vSync;
	}

	void glGraphicsContext::SetDebugMessageCallback()
	{
#if   defined LIGHT_DIST
		// disable all messages except GL_DEBUG_HIGH_SEVERITY
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
#elif defined LIGHT_DEBUG 
		// synchronous output will affect the performance, enabled only in debug configurations
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		                          const GLchar* msg, const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH: 
				LT_DBREAK; // __debugbreak here so we can see where we are in the call stack
				// #todo: determine if we should throw glException or not
				throw glException(source, type, id, msg);
					
			case GL_DEBUG_SEVERITY_MEDIUM: case GL_DEBUG_SEVERITY_LOW:
				LT_CORE_WARN("glMessageCallback: Severity: {} :: Source: {} :: Type: {} :: ID: {}",
				             glToString::DebugMsgSeverity(severity),
				             glToString::DebugMsgSource  (source  ),
				             glToString::DebugMsgType    (type    ), id);
				LT_CORE_WARN("        {}", msg);
				return;

			default:
				LT_CORE_TRACE("glMessageCallback: Severity: {} :: Source: {} :: Type: {} :: ID: {}",
				              glToString::DebugMsgSeverity(severity),
				              glToString::DebugMsgSource  (source  ),
				              glToString::DebugMsgType    (type    ), id);
				LT_CORE_TRACE("        {}", msg);
				return;
			}
		}, nullptr);
	}

}