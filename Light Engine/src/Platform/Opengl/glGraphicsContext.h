#pragma once

#include "Renderer/GraphicsContext.h"

#include "Core/Core.h"

struct GLFWwindow;

namespace Light {

	class Event;
	class WindowResizedEvent;

	class glGraphicsContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_WindowHandle;
	public:
		glGraphicsContext(std::shared_ptr<Window> game_window, GraphicsConfigurations data);

		void HandleWindowEvents(Event& event) override;

		void EnableVSync () override;
		void DisableVSync() override;

		void SwapBuffers() override;

		void Clear() override;
		void ClearBuffer(float r, float g, float b, float a) override;

		void Draw(unsigned int count) override;
		void DrawIndexed(unsigned int count) override;

	private:
		bool OnWindowResize(WindowResizedEvent& event);
	};

}