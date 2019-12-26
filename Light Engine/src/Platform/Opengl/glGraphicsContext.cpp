#include "ltpch.h"
#include "glGraphicsContext.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Light {

	glGraphicsContext::glGraphicsContext(std::shared_ptr<Window> game_window, bool v_sync)
	{
		m_WindowHandle = game_window->GetHandle();
		b_VSync = v_sync;

		glfwMakeContextCurrent(game_window->GetHandle());
		glfwSwapInterval(v_sync);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		LT_CORE_INFO("glGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", glGetString(GL_RENDERER));
		LT_CORE_INFO("        Version : {}", glGetString(GL_VERSION));
	}

	glGraphicsContext::~glGraphicsContext()
	{
		glfwMakeContextCurrent(NULL);
	}

	void glGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void glGraphicsContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void glGraphicsContext::ClearBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void glGraphicsContext::HandleWindowEvents(Event& event)
	{
	}

}