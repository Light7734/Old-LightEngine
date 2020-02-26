#include <LightEngine.h>
#include <Core/EntryPoint.h>

class Sandbox : public Light::Application
{
public:
	Sandbox()
	{
		// Create window data
		Light::WindowData wd;
		wd.title = "Demo";
		wd.displayMode = Light::DisplayMode::Windowed;

		// Create graphics configurations
		Light::GraphicsConfigurations gc;
		gc.resolution = Light::Resolution(800.0f, 600.0f, Light::AspectRatio::AR_4_3);
		gc.vSync = true;

		// Create window and set callback function
		m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Default);
		m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));
	}
};

Light::Application* Light::CreateApplication()
{
	return new Sandbox;
}