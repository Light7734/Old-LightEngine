#include "ltpch.h"
#include "Application.h"

#include "Window.h"
#include "Timer.h"

#include "Layers/Layer.h"

#include "Input/Input.h"

#include "Events/WindowEvents.h"

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
		while (m_Window->isRunning())
		{
			Time::Update();
			RenderCommand::Clear();

			for (auto layer : m_Layers)
				layer->OnUpdate(Time::GetDeltaTime());

			for (auto layer : m_Layers)
				layer->OnRender();

			// UserInterface::Begin(); #todo: Add ImGui
			for (auto layer : m_Layers)
				layer->OnUserInterfaceUpdate();
			// UserInterface::End  (); #todo: Add ImGui


			m_Window->HandleEvents();
			RenderCommand::SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event)
	{
		LT_CORE_TRACE(event.GetLogInfo());

		if(event.isInCategory(EventCategory_Input))
			Input::OnEvent(event);

		if (event.isInCategory(EventCategory_Window))
			RenderCommand::HandleWindowEvents(event);

		for (auto it = m_Layers.end(); it != m_Layers.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.b_Dispatched)
				return;
		}
	
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(LT_EVENT_FN(Application::OnWindowClosedEvent));
	}

	void Application::AttachLayer(std::shared_ptr<Layer> layer)
	{
		m_Layers.push_back(layer);
		layer->OnAttach();
	}

	void Application::DetatchLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end())
			{ layer->OnDetatch(); m_Layers.erase(it); }
		else
			LT_CORE_ERROR("failed to find the specified layer from LayerStack");
	}

	bool Application::OnWindowClosedEvent(WindowClosedEvent& event)
	{
		m_Window->Close();
		return true;
	}

}