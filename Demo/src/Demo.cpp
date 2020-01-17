#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

Demo::Demo()
{
	LT_TRACE("Constructing Demo");

	// Create window data & initialize window
	Light::WindowData wd = { "Demo", Light::DisplayMode::Windowed, 800, 600 };

	m_Window = std::make_unique<Light::Window>(wd);
	m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));

	// Create graphics configurations & initialize graphics context
	Light::GraphicsConfigurations gc = { true };
	Light::GraphicsContext::CreateContext(Light::GraphicsAPI::Opengl, gc);

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