#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

Demo::Demo()
{
	LT_TRACE("Constructing Demo");

	// Create window data to initialize window
	Light::WindowData data = { "Demo", Light::DisplayMode::Windowed, Light::WindowState::Visible, 400, 400, 100, 100, false };

	m_Window = Light::Window::Create(data);
	m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));

	// Create graphic context
	Light::GraphicsContext::SetAPI(Light::GraphicsAPI::DirectX);
	Light::GraphicsContext::Init(m_Window);

	// Attach test layer
	AttachLayer(std::make_shared<DemoLayer>());
}

Demo::~Demo()
{
	LT_TRACE("Destructing Demo");
}

Light::Application* Light::CreateApplication()
{
	return new Demo;
}