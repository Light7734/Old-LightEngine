#include "DemoLayer.h"

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached DemoLayer");
	m_GameWindow = Light::Application::GetGameWindow();

	m_TriangleLayer = new TriangleLayer;
	Light::LayerStack::AttachLayer(m_TriangleLayer);
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detatched DemoLayer");
}

void DemoLayer::OnUpdate(float deltaTime)
{
	static Light::Timer timer;
	const float c = abs(sin(timer.ElapsedTime()));
	Light::RenderCommand::ClearBuffer(0.3f, 0.1, c, 1.0f);

	if (Light::Input::GetKey(KEY_ENTER))
		LT_TRACE("Enter key is down!");
}

void DemoLayer::OnEvent(Light::Event& event)
{
	// Uncomment to log every event
	// LT_TRACE(event.GetLogInfo());

	Light::Dispatcher dispatcher(event);
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
	dispatcher.Dispatch<Light::MouseMovedEvent>(LT_EVENT_FN(DemoLayer::OnMouseMove));
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
	
		Light::LayerStack::DetatchLayer(m_TriangleLayer);
		Light::LayerStack::AttachLayer(m_TriangleLayer);
	}

	if (event.GetKey() == KEY_D)
		m_TriangleLayer->Disable();
	if (event.GetKey() == KEY_E)
		m_TriangleLayer->Enable();


	return true;
}

bool DemoLayer::OnMouseMove(Light::MouseMovedEvent& event)
{
	if (event.GetX() > 200 && event.GetX() < 600 && event.GetY() > 150 && event.GetY() < 450)
		m_TriangleLayer->Disable();
	else
		m_TriangleLayer->Enable();

	return true;
}
