#include "ltpch.h"
#include "glGraphicsContext.h"

#include "Core/Window.h"

#include "Debug/Exceptions.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "glDebug/glToString.h"

namespace Light {

	glGraphicsContext::glGraphicsContext(const GraphicsConfigurations& configurations)
		: m_WindowHandle(Window::GetGlfwHandle())
	{
		m_Configurations = configurations;

		glfwMakeContextCurrent(m_WindowHandle);
		glfwSwapInterval(m_Configurations.vSync);
		LT_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "gladLoadGLLoader failed");

		// Refresh display mode (because dxGraphicsContext's dtor sets SwapChain fullscreen state to false resulting in windowed window)
		Window::SetDisplayMode(Window::GetDisplayMode());
		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if   defined LIGHT_DIST
		// Disable all messages except GL_DEBUG_HIGH_SEVERITY
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
#elif defined LIGHT_DEBUG 
		// Synchronous output will affect performance, enabled only in debug cfg
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		                          const GLchar* msg, const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH: 
				LT_DBREAK; // Break here so we can see where we are in the Call stack
				// #todo: determine if we should actually throw glException or not
				throw glException(source, type, id, msg);

			case GL_DEBUG_SEVERITY_MEDIUM: case GL_DEBUG_SEVERITY_LOW:
				LT_CORE_WARN("GLMessage :: Severity: {} :: Source: {} :: Type: {} :: ID: {}",
				             glToString::DebugMsgSeverity(severity),
				             glToString::DebugMsgSource  (source  ),
				             glToString::DebugMsgType    (type    ), id);
				LT_CORE_WARN("        {}", msg);
				return;

			default:
				LT_CORE_TRACE("GLMessage :: Severity: {} :: Source: {} :: Type: {} :: ID: {}",
				              glToString::DebugMsgSeverity(severity),
				              glToString::DebugMsgSource  (source  ),
				              glToString::DebugMsgType    (type    ), id);
				LT_CORE_TRACE("        {}", msg);
				return;
			}
		}, nullptr);

		// #todo: Log more information
		LT_CORE_INFO("glGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", glGetString(GL_RENDERER));
		LT_CORE_INFO("        Version : {}", glGetString(GL_VERSION ));
	}
	
	void glGraphicsContext::HandleWindowEvents(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizedEvent>(LT_EVENT_FN(glGraphicsContext::OnWindowResize));
	}

	void glGraphicsContext::EnableVSync()
	{
		m_Configurations.vSync = true;
		glfwSwapInterval(1);
	}

	void glGraphicsContext::DisableVSync()
	{
		m_Configurations.vSync = false;
		glfwSwapInterval(0);
	}

	void glGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void glGraphicsContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void glGraphicsContext::ClearBuffer(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void glGraphicsContext::Draw(unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void glGraphicsContext::DrawIndexed(unsigned int count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	bool glGraphicsContext::OnWindowResize(WindowResizedEvent& event)
	{
		glViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}

}