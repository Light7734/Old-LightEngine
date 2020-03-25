#include "ltpch.h"
#include "Application.h"

#include "Timer.h"
#include "Window.h"
#include "Monitor.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include "Input/Input.h"

#include "Layers/Layer.h"

#include "Renderer/Buffers.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Logger::Init();

		LT_CORE_ASSERT(!s_Instance, "Application::Application: multiple Application instances");
		s_Instance = this;
	}

	Application::~Application()
	{
		s_Instance = nullptr;

		LT_FILE_INFO("Application::~Application: total application runtime: {}s", Time::ElapsedTime());
		Logger::Terminate();
	}

	void Application::GameLoop()
	{
		LT_CORE_ASSERT(m_Window, "Application::GameLoop: Application::m_Window is not initialized");
		m_LayerStack.HandleQueuedLayers();

		while (!m_Window->IsClosed())
		{
			Time::CalculateDeltaTime();
			RenderCommand::ClearBackbuffer();


			// update
			for (Layer* layer : m_LayerStack)
				if(layer->IsEnabled()) 
					layer->OnUpdate(Time::GetDeltaTime());
			m_LayerStack.HandleQueuedLayers();

			if (!m_Window->IsMinimized())
			{
				// render
				Renderer::Begin();
				for (Layer* layer : m_LayerStack)
					if (layer->IsEnabled())
						layer->OnRender();
				Renderer::End();

				// user interface
				UserInterface::Begin();
				for (Layer* layer : m_LayerStack)
					if (layer->IsEnabled())
						layer->OnUserInterfaceUpdate();
				m_LayerStack.HandleQueuedLayers();
				UserInterface::End();
			}


			m_Window->HandleEvents();
			RenderCommand::SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event)
	{
		if (event.IsInCategory(EventCategory_Input))
			Input::OnEvent(event);

		if(event.GetEventType() == EventType::WindowMoved)
			Monitor::OnWindowMove();

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if ((*it)->IsEnabled())
			{
				(*it)->OnEvent(event);
				if (event.IsDispatched())
					return;
			}
		}
	
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(LT_EVENT_FN(Application::OnWindowClosedEvent));
	}

	bool Application::OnWindowClosedEvent(WindowClosedEvent& event)
	{
		m_Window->Close();
		return true;
	}

}