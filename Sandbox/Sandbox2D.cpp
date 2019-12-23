#include <LightEngine.h>
#include <Core/EntryPoint.h>

#include <Platform/Windows/WindowsWindow.h>

#include <Events/Event.h>
#include <Events/KeyboardEvents.h>
#include <Events/WindowEvents.h>

class Sandbox2D : public Light::Application 
{
public:
	Sandbox2D()  
	{ 
		LT_DEBUG("Constructing Sandbox2D");

		Light::WindowData data = { "Light Engine", Light::WindowState::Normal, 500, 400, 200, 20, false };

		m_Window = Light::Window::Create(data, false);
		m_Window->SetEventCallbackFunction(std::bind(&Light::Application::OnEvent, this, std::placeholders::_1));

		Light::GraphicsContext::SetAPI(Light::GraphicsAPI::DirectX);
		Light::GraphicsContext::Init(m_Window.get());
	}	
	~Sandbox2D() { LT_DEBUG("Destructing Sandbox2D");  }

};


Light::Application* Light::CreateApplication()
{
	return new Sandbox2D;
}