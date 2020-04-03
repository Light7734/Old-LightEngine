#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // this must be included in only one cpp file

class t : public Light::Layer
{
public:
	t() { LT_TRACE("t construct"); }
	void OnAttach() override { LT_TRACE("t attach!"); }
	void OnDetatch() override { LT_TRACE("t detatch!"); }
};

Demo::Demo()
{
	// seed rand
	srand(time(NULL));


	// create window data
	Light::WindowData wd;
	wd.title       = "Demo";
	wd.displayMode = Light::DisplayMode::Windowed;

	// create graphics configurations
	Light::GraphicsConfigurations gc;
	gc.resolution = Light::Resolution(1280, 720, Light::AspectRatio::AR_16_9);
	gc.vSync      = false;

	// create window and set callback function
	m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Directx);
	m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));
	
	Light::Blender::Enable();

	Light::Application::AttachLayer(new DemoLayer);
}

Demo::~Demo()
{
	LT_TRACE("Destructing Demo");
}

Light::Application* Light::CreateApplication()
{
	return new Demo;
}