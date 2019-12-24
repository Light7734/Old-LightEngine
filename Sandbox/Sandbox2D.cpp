#include <LightEngine.h>
#include <Core/EntryPoint.h>

class ClearBufferLayer : public Light::Layer 
{
private:
	Light::Timer timer;
public:
	void OnUpdate(float DeltaTime) override
	{
		const auto c = std::abs(sin(timer.ElapsedTime()));
		const auto c2 = std::abs(cos(timer.ElapsedTime()));
		LT_TRACE(c);
		Light::RenderCommand::ClearBuffer(c * 127.0f, c * 255.0f, c2 * 255, 255);
	}
};

class Sandbox2D : public Light::Application 
{
public:
	Sandbox2D()  
	{ 
		LT_DEBUG("Constructing Sandbox2D");

		Light::WindowData data = { "Light Engine", Light::DisplayMode::Windowed, Light::WindowState::Visible, 500, 400, 200, 20, false };

		m_Window = Light::Window::Create(data);
		m_Window->SetEventCallbackFunction(std::bind(&Light::Application::OnEvent, this, std::placeholders::_1));

		Light::GraphicsContext::SetAPI(Light::GraphicsAPI::DirectX);
		Light::GraphicsContext::Init(m_Window);

		AttachLayer(std::make_shared<ClearBufferLayer>());
	}	
	~Sandbox2D() { LT_DEBUG("Destructing Sandbox2D");  }

};


Light::Application* Light::CreateApplication()
{
	return new Sandbox2D;
}