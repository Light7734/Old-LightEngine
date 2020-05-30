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
		LT_PROFILE_FUNC();

		Logger::Init();
		srand(time(NULL));

		LT_CORE_ASSERT(!s_Instance, "Application::Application: multiple Application instances");
		s_Instance = this;
	}

	void Application::AttachLayer(Layer* layer)
	{
		s_Instance->m_LayerStack.AttachLayer(layer);
		layer->OnAttach();
	}

	void Application::DetachLayer(Layer* layer)
	{
		s_Instance->m_LayerStack.DetachLayer(layer);
		layer->OnDetatch();
	}

	Application::~Application()
	{
		LT_PROFILE_FUNC();

		s_Instance = nullptr;

		LT_FILE_INFO("Application::~Application: total application runtime: {}s", Time::ElapsedTime());
		Logger::Terminate();
	}

	void Application::GameLoop()
	{
		LT_PROFILE_FUNC();

		LT_CORE_ASSERT(m_Window, "Application::GameLoop: Application::m_Window is not initialized");
		LT_CORE_ASSERT(m_LayerStack.GetSize(), "Application::GameLoop: LayerStack has no attached layers");
		m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));

		while (!m_Window->IsClosed())
		{
			Time::CalculateDeltaTime();

			{
				// update
				LT_PROFILE_SCOPE("Application::GameLoop::OnUpdate");
				for (const auto& it = m_LayerStack.begin(); it != m_LayerStack.end(); m_LayerStack.next())
					if ((*it)->IsEnabled())
						(*it)->OnUpdate(Time::GetDeltaTime());
			}

			if (!m_Window->IsMinimized())
			{
				{
					// render
					LT_PROFILE_SCOPE("Application::GameLoop::OnRender");
					Renderer::BeginFrame();
					for (const auto& it = m_LayerStack.begin(); it != m_LayerStack.end(); m_LayerStack.next())
						if ((*it)->IsEnabled())
							(*it)->OnRender();
					Renderer::EndFrame();
				}

				{
					// user interface
					LT_PROFILE_SCOPE("Application::GameLoop::OnUserInterface");
					UserInterface::Get()->Begin();
					for (const auto& it = m_LayerStack.begin(); it != m_LayerStack.end(); m_LayerStack.next())
						if ((*it)->IsEnabled())
							(*it)->OnUserInterfaceUpdate();
					UserInterface::Get()->End();
				}
			}

			{
				// handle event
				LT_PROFILE_SCOPE("Application::GameLoop::HandleEvents");
				m_Window->HandleEvents();
			}

			{
				// swap buffers and clear buffer
				LT_PROFILE_SCOPE("Application::GameLoop::RenderCommand");
				RenderCommand::SwapBuffers();
				RenderCommand::ClearBackbuffer();
			}
		}
	}

	void Application::OnEvent(Event& event)
	{
		if (event.IsInCategory(EventCategory_Input))
			Input::OnEvent(event);

		if(event.GetEventType() == EventType::WindowMoved)
			Monitor::OnWindowMove();

		for (const auto& it = m_LayerStack.rbegin(); it != m_LayerStack.rend();)
		{
			m_LayerStack.previous();

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