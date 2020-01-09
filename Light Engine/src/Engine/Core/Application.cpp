#include "ltpch.h"
#include "Application.h"

#include "Timer.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include "Input/Input.h"

#include "Layers/Layer.h"

#include "Renderer/RenderCommand.h"

namespace Light {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		LT_CORE_ASSERT(!s_Instance, EC_MULTIPLE_INSTANCES_APPLICATION, "Multiple Application instances");
		s_Instance = this;

		Logger::Init();
	}

	Application::~Application()
	{
		s_Instance = nullptr;

		LT_FILE_INFO("Total application runtime: {}s", Time::ElapsedTime());
		Logger::Terminate();
	}

	void Application::GameLoop()
	{
		LT_CORE_ASSERT(m_Window, EC_NO_INIT_WINDOW, "Application::m_Window is not initialized");
		LT_CORE_ASSERT(RenderCommand::isInitialized(), EC_NO_INIT_GRAPHICSC_CONTEXT, "GraphicsContext::Init() was never called");

		while (m_Window->isRunning())
		{
			Time::Update();
			RenderCommand::Clear();


			for (Layer* layer : m_LayerStack)
				if(layer->IsEnable()) 
					layer->OnUpdate(Time::GetDeltaTime());

			for (Layer* layer : m_LayerStack)
				if (layer->IsEnable())
					layer->OnRender();

			// UserInterface::Begin(); #todo: Add ImGui
			for (Layer* layer : m_LayerStack)
				if (layer->IsEnable())
					layer->OnUserInterfaceUpdate();
			// UserInterface::End  (); #todo: Add ImGui


			m_LayerStack.HandleQueuedLayers();
			m_Window->HandleEvents();

			RenderCommand::SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event)
	{
		if(event.isInCategory(EventCategory_Input))
			Input::OnEvent(event);

		if (event.isInCategory(EventCategory_Window))
			RenderCommand::HandleWindowEvents(event);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if((*it)->IsEnable())
				(*it)->OnEvent(event);

			if (event.b_Dispatched)
				return;
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