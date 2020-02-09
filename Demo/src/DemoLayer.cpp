#include "DemoLayer.h"

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached DemoLayer");

	LT_INFO("------------------------------------");
	LT_INFO("|    KEY    |        ACTION        |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|    ESC    |         Exit         |");
	LT_INFO("|     1     |  BorderlessWindowed  |");
	LT_INFO("|     2     |       Windowed       |");
	LT_INFO("|     3     |      Fullscreen      |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|     E     |    GraphicsAPI:GL    |");
	LT_INFO("|     R     |    GraphicsAPI:DX    |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|     W     |      Move: Up        |");
	LT_INFO("|     A     |      Move: Left      |");
	LT_INFO("|     S     |      Move: Down      |");
	LT_INFO("|     D     |      Move: Right     |");
	LT_INFO("------------------------------------");

	m_Quad = new QuadLayer;
	Light::LayerStack::AttachLayer(m_Quad);

	Light::Camera::SetPosition(glm::vec2(0.0f, 0.0f));
	Light::Camera::SetProjection(Light::Window::GetWidth(), Light::Window::GetHeight());
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detatched DemoLayer");
}

void DemoLayer::OnUpdate(float deltaTime)
{
	static Light::Timer timer;
	Light::RenderCommand::ClearBuffer(0.0f, 0.0, abs(sin(timer.ElapsedTime())), 1.0f);

	if (Light::Input::GetMouseX() > m_Quad->GetPosition().x&&
		Light::Input::GetMouseX() < m_Quad->GetPosition().x + m_Quad->GetSize().x &&
		Light::Input::GetMouseY() > m_Quad->GetPosition().y&&
		Light::Input::GetMouseY() < m_Quad->GetPosition().y + m_Quad->GetSize().y)
		m_Quad->Disable();
	else
		m_Quad->Enable();
}

void DemoLayer::OnEvent(Light::Event& event)
{
	// uncomment to log every event
	// LT_TRACE(event.GetLogInfo());

	if (event.IsInCategory(Light::EventCategory_Window)) // log all window events
		LT_WARN(event.GetLogInfo());

	Light::Dispatcher dispatcher(event); 
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
}

bool DemoLayer::OnKeyPress(Light::KeyboardKeyPressedEvent& event)
{
	if (event.GetKey() == KEY_ESCAPE)
		Light::Window::Close();

	if (event.GetKey() == KEY_E || event.GetKey() == KEY_R)
	{
		Light::GraphicsContext::CreateContext(event.GetKey() == KEY_E ?
		                                      Light::GraphicsAPI::Opengl : Light::GraphicsAPI::Directx, { true });
		Light::LayerStack::DetatchLayer(m_Quad);
		Light::LayerStack::AttachLayer(m_Quad);
	}

	if (event.GetKey() == KEY_1)
		Light::Window::SetDisplayMode(Light::DisplayMode::BorderlessWindowed);

	if (event.GetKey() == KEY_2)
		Light::Window::SetDisplayMode(Light::DisplayMode::Windowed);

	if (event.GetKey() == KEY_3)
		Light::Window::SetDisplayMode(Light::DisplayMode::Fullscreen);

	return true;
}