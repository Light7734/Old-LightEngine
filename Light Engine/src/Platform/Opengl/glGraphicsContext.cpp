#include "ltpch.h"
#include "glGraphicsContext.h"

#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Light {

	glGraphicsContext::glGraphicsContext(std::shared_ptr<Window> game_window, GraphicsData data)
	{
		m_WindowHandle = game_window->GetHandle();
		m_Data = data;

		glfwMakeContextCurrent(game_window->GetHandle());
		glfwSwapInterval(data.vSync);
		LT_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), EC_CALL_FAIL_GLAD_LOAD, "gladLoadGLLoader failed");

		LT_CORE_INFO("glGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", glGetString(GL_RENDERER));
		LT_CORE_INFO("        Version : {}", glGetString(GL_VERSION ));
	}

	glGraphicsContext::~glGraphicsContext()	
	{
		LT_CORE_DEBUG("Destructing glGraphicsContext");
	}

	void glGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void glGraphicsContext::EnableVSync()
	{
		m_Data.vSync = true;
		glfwSwapInterval(1);
	}

	void glGraphicsContext::DisableVSync()
	{
		m_Data.vSync = false;
		glfwSwapInterval(0);
	}

	void glGraphicsContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void glGraphicsContext::ClearBuffer(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void glGraphicsContext::HandleWindowEvents(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizedEvent>(LT_EVENT_FN(glGraphicsContext::OnWindowResize));
	}

	bool glGraphicsContext::OnWindowResize(WindowResizedEvent& event)
	{
		glViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}

}