#pragma once

#include "Layers/LayerStack.h"

#include "Core/Core.h"

namespace Light {

	class Window;
	class Layer;

	class Event;
	class WindowClosedEvent;

	class Application
	{
	private:
		static Application* s_Instance;
		LayerStack m_LayerStack;
	protected:
		std::shared_ptr<Window> m_Window;
	public:
		Application           (                  )         ;
		Application           (const Application&) = delete;
		Application& operator=(const Application&) = delete;
		virtual ~Application  (                  )         ;

		void GameLoop(            );
		void OnEvent (Event& event);

		static inline std::shared_ptr<Window> GetGameWindow() { return s_Instance->m_Window; }
	private:
		bool OnWindowClosedEvent(WindowClosedEvent& event);
	};

	extern Application* CreateApplication();

}