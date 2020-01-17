#include "ltpch.h"
#include "Application.h"

#include "Timer.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include "Input/Input.h"

#include "Layers/Layer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace Light {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Logger::Init();

		LT_CORE_ASSERT(!s_Instance, "Multiple Application instances");
		s_Instance = this;
	}

	Application::~Application()
	{
		s_Instance = nullptr;

		LT_FILE_INFO("Total application runtime: {}s", Time::ElapsedTime());
		Logger::Terminate();
	}

	void Application::GameLoop()
	{
		LT_CORE_ASSERT(m_Window, "Application::m_Window is not initialized");
		LT_CORE_ASSERT(RenderCommand::IsInitialized(), "GraphicsContext::CreateContext() was never called");

		while (!m_Window->IsClosed())
		{
			Time::Update();
			RenderCommand::Clear();


			for (Layer* layer : m_LayerStack)
				if(layer->IsEnable()) 
					layer->OnUpdate(Time::GetDeltaTime());

			Renderer::Start();
			for (Layer* layer : m_LayerStack)
				if (layer->IsEnable())
					layer->OnRender();
			Renderer::End();

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