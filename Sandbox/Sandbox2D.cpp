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
		wd.displayMode = Light::DisplayMode::Windowed;

		m_Window = std::make_unique<Light::Window>(wd);
		m_Window->SetEventCallbackFunction(LT_EVENT_FN(Light::Application::OnEvent));

		// Initialize Graphics
		Light::GraphicsConfigurations gc;
		gc.vSync = true;

		Light::GraphicsContext::CreateContext(Light::GraphicsAPI::Opengl, gc);
		Light::Camera::SetProjection(wd.width, wd.height);
		Light::Camera::SetPosition(glm::vec2(0.0f, 0.0f));
	}
	};

Light::Application* Light::CreateApplication()
{
	return new Sandbox;
}