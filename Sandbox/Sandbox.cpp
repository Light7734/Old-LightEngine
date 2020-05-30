#include <LightEngine.h>
#include <Core/EntryPoint.h>

class dummyLayer : public Light::Layer {};

class Sandbox : public Light::Application
{
public:
	Sandbox()
	{
		// initialize window
		Light::WindowData wd;
		wd.title = "Sandbox";
		wd.displayMode = Light::DisplayMode::Windowed;

		Light::GraphicsConfigurations gc;
		gc.resolution = Light::Resolution(800.0f, 600.0f, Light::AspectRatio::AR_4_3);

		m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Default);

		// attach da dumb layer
		Light::Application::AttachLayer(new dummyLayer());
	}
};

Light::Application* Light::CreateApplication()
{
	return new Sandbox;
}