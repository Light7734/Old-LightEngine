#include "ltpch.h"
#include "glGraphicsContext.h"

#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

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

#ifndef LIGHT_DIST
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* prm)
		{
			// #todo: improve:
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				type, severity, msg);
		}, NULL);
#endif

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