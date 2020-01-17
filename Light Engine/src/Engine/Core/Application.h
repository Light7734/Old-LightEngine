#pragma once

#include "Layers/LayerStack.h"

#include "Core/Core.h"

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
		Application           (                  )         ;
		Application           (const Application&) = delete;
		Application& operator=(const Application&) = delete;
		virtual ~Application  (                  )         ;

		void GameLoop(            );
		void OnEvent (Event& event);
	private:
		bool OnWindowClosedEvent(WindowClosedEvent& event);
	};

	extern Application* CreateApplication();

}