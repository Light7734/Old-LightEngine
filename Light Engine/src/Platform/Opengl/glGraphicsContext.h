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
		glGraphicsContext(const GraphicsConfigurations& configurations);

		void SwapBuffers() override;

		void ClearBackbuffer(float colors[4]) override;

		void Draw(unsigned int count) override;
		void DrawIndexed(unsigned int count) override;

		// Setters
		void SetConfigurations(const GraphicsConfigurations& configurations) override;
		void SetResolution(const Resolution& resolution) override;
		void SetVSync(bool vSync) override;

	private:
		void SetDebugMessageCallback();
	};

}