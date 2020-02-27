#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

Demo::Demo()
{
	// Create window data
	Light::WindowData wd;
	wd.title       = "Demo";
	wd.displayMode = Light::DisplayMode::Windowed;

	// Create graphics configurations
	Light::GraphicsConfigurations gc;
	gc.resolution = Light::Resolution(800.0f, 600.0f, Light::AspectRatio::AR_4_3);
	gc.vSync      = false;

	// Create window and set callback function
	m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Opengl);
	m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));


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