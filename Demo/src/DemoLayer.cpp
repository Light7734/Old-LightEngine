#include "DemoLayer.h"

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached DemoLayer");
	m_GameWindow = Light::Application::GetGameWindow();

	m_TriangleLayer = std::make_shared<TriangleLayer>();
	Light::Application::AttachLayer(m_TriangleLayer);
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detatched DemoLayer");
}

void DemoLayer::OnUpdate(float deltaTime)
{
	static Light::Timer timer;
	static int frames = 0;
	static int fpsLogTime = 1;

	frames++;
	if (timer.ElapsedTime() >= fpsLogTime)
	{
		fpsLogTime++;
		LT_DEBUG("FPS: {}", frames);
		frames = 0;
	}

	const float c = abs(sin(timer.ElapsedTime()));
	Light::RenderCommand::ClearBuffer(0.3f, 0.1, c, 1.0f);


	if (Light::Input::GetKey(KEY_ENTER))
		LT_TRACE("Enter key is down!");
}

void DemoLayer::OnEvent(Light::Event& event)
{
	LT_TRACE(event.GetLogInfo());

	Light::Dispatcher dispatcher(event);
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
}

bool DemoLayer::OnKeyPress(Light::KeyboardKeyPressedEvent& event)
{
	if (event.GetKey() == KEY_ESCAPE)
		m_GameWindow->Close();

	if (event.GetKey() == KEY_KP_1 || event.GetKey() == KEY_KP_2)
	{
		Light::GraphicsContext::Init(event.GetKey() == KEY_KP_1 ?
		                             Light::GraphicsAPI::Opengl : Light::GraphicsAPI::DirectX,
		                             { true }, m_GameWindow);
	
		Light::Application::DetatchLayer(m_TriangleLayer);
		Light::Application::AttachLayer(m_TriangleLayer);
	}

	return true;
}