#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

Demo::Demo()
{
	LT_PROFILE_FUNC();
	LT_TRACE("Demo::Demo");

	// initialize window
	Light::WindowData wd;
	wd.title = "Demo";
	wd.displayMode = Light::DisplayMode::Windowed;

	Light::GraphicsConfigurations gc;
	gc.resolution = Light::Resolution(1280u, 720u, Light::AspectRatio::AR_16_9);

	m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Directx);

	// attach demo layer
	Light::Application::AttachLayer(new DemoLayer);
}

Demo::~Demo()
{
	LT_TRACE("Demo::~Demo");
}

Light::Application* Light::CreateApplication()
{
	return new Demo;
}