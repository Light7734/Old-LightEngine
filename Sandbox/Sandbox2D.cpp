#include <LightEngine.h>
#include <Core/EntryPoint.h>


class Sandbox : public Light::Application
{
public:
	Sandbox()
	{
		// Create Window
		Light::WindowData wd;
		wd.title = "Sandbox";
		wd.width = 800;
		wd.height = 600;
		wd.x = 200;
		wd.y = 200;
		wd.displayMode = Light::DisplayMode::Windowed;
		wd.eventCallback = LT_EVENT_FN(Light::Application::OnEvent);

		m_Window = std::make_shared<Light::Window>(wd);

		// Initialize Graphics
		Light::GraphicsConfigurations gc;
		gc.vSync = true;

		Light::GraphicsContext::Init(Light::GraphicsAPI::Opengl, gc, m_Window);
	}
	
	~Sandbox()
	{

	}
};

Light::Application* Light::CreateApplication()
{
	return new Sandbox;
}