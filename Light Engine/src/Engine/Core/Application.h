#pragma once

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
	protected:
		std::shared_ptr<Window> m_Window;
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	public:
		Application           (                  )        ;
		Application           (const Application&) = delete;
		Application& operator=(const Application&) = delete;
		virtual ~Application  (                  )         ;

		void GameLoop(            );
		void OnEvent (Event& event);

		void AttachLayer (std::shared_ptr<Layer> layer);
		void DetatchLayer(std::shared_ptr<Layer> layer);

		static inline std::shared_ptr<Window> GetGameWindow() { return s_Instance->m_Window; }
	private:
		bool OnWindowClosedEvent(WindowClosedEvent& event);
	};

	extern Application* CreateApplication();

}