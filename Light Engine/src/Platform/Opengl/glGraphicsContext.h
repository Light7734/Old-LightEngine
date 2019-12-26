#pragma once

#include "Renderer/GraphicsContext.h"

#include "Core/Core.h"

struct GLFWwindow;

namespace Light {

	class Event;

	class glGraphicsContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_WindowHandle;
	public:
		glGraphicsContext(std::shared_ptr<Window> game_window, bool v_sync);
		~glGraphicsContext();

		void SwapBuffers() override;

		void Clear() override;
		void ClearBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

		void HandleWindowEvents(Event& event) override;
	};

}