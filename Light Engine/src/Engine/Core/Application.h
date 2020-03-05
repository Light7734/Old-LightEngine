#pragma once

#include "Core.h"

#include "Layers/LayerStack.h"

namespace Light {

	class Window;

	class Event;
	class WindowClosedEvent;

	class Application
	{
	private:
		static Application* s_Instance;
		LayerStack m_LayerStack;
	protected:
		std::unique_ptr<Window> m_Window;
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void GameLoop();
		void OnEvent(Event& event);
	private:
		bool OnWindowClosedEvent(WindowClosedEvent& event);
	};

	extern Application* CreateApplication();

}