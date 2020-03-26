#include <LightEngine.h>
#include <Core/EntryPoint.h>

class Sandbox : public Light::Application
{
public:
	Sandbox()
	{
		// create window data
		Light::WindowData wd;
		wd.title = "Demo";
		wd.displayMode = Light::DisplayMode::Windowed;

		// create graphics configurations
		Light::GraphicsConfigurations gc;
		gc.resolution = Light::Resolution(800.0f, 600.0f, Light::AspectRatio::AR_4_3);
		gc.vSync = true;

		// create window and set callback function
		m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Default);
		m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));
	}
};

Light::Application* Light::CreateApplication()
{
	return new Sandbox;
}