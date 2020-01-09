#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

Demo::Demo()
{
	LT_TRACE("Constructing Demo");

	// Create window data to initialize window
	Light::WindowData wd = { "Demo", Light::DisplayMode::Windowed, Light::WindowState::Visible, 800, 600, 100, 100 };

	m_Window = std::make_shared<Light::Window>(wd);
	m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));

	// Create graphics configurations to initialize graphics context
	Light::GraphicsConfigurations gc = { true };
	Light::GraphicsContext::Init(Light::GraphicsAPI::Opengl, gc, m_Window);

	// Attach the DemoLayer
	Light::LayerStack::AttachLayer(new DemoLayer);
}

Demo::~Demo()
{
	LT_TRACE("Destructing Demo");
}

Light::Application* Light::CreateApplication()
{
	return new Demo;
}