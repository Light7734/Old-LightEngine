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
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	protected:
		std::shared_ptr<Window> m_Window;
	public:
		Application           (                  )         ;
		Application           (const Application&) = delete;
		Application& operator=(const Application&) = delete;
		virtual ~Application  (                  )         ;

		void GameLoop(            );
		void OnEvent (Event& event);

		static inline void AttachLayer(std::shared_ptr<Layer> layer)  { s_Instance->AttachLayerImpl(layer); }
		static inline void DetatchLayer(std::shared_ptr<Layer> layer) { s_Instance->DetatchLayerImpl(layer); }

		static inline std::shared_ptr<Window> GetGameWindow() { return s_Instance->m_Window; }
	private:
		void AttachLayerImpl (std::shared_ptr<Layer> layer);
		void DetatchLayerImpl(std::shared_ptr<Layer> layer);

		bool OnWindowClosedEvent(WindowClosedEvent& event);
	};

	extern Application* CreateApplication();

}