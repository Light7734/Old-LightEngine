#include "DemoLayer.h"

#include <functional>

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached DemoLayer");
	m_GameWindow = Light::Application::GetGameWindow();
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detatched DemoLayer");
}

void DemoLayer::OnUpdate(float deltaTime)
{
	static Light::Timer timer; // timer class saves the start time point on construction
	static uint32_t frames = 0u;
	static uint32_t fpsLogTime = 1u;

	frames++;
	if (timer.ElapsedTime() >= fpsLogTime++)
	{
		LT_DEBUG("FPS: {}", frames);
		frames = 0;
	}

	const float c = abs(sin(timer.ElapsedTime()));
	const float c2 = abs(cos(timer.ElapsedTime()));
	Light::RenderCommand::ClearBuffer(c2 * 177, c * 255, c2 * 255, 255);


	if (Light::Input::GetKey(KEY_ENTER))
		LT_TRACE("Enter key is down!");
}

void DemoLayer::OnRender()
{
	// Light engine doesn't have a renderer yet
}

void DemoLayer::OnEvent(Light::Event& event)
{
	// LT_TRACE(event.GetLogInfo()); // log every events

	Light::Dispatcher dispatcher(event);
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
	dispatcher.Dispatch<Light::MouseScrolledEvent>(LT_EVENT_FN(DemoLayer::OnMouseScroll));
}

bool DemoLayer::OnKeyPress(Light::KeyboardKeyPressedEvent& event)
{
	if (event.GetKey() == KEY_1)
		m_GameWindow->SetDisplayMode(Light::DisplayMode::BorderlessWindowed);
	if (event.GetKey() == KEY_2)
		m_GameWindow->SetDisplayMode(Light::DisplayMode::ExclusiveFullscreen);
	if (event.GetKey() == KEY_3)
		m_GameWindow->SetDisplayMode(Light::DisplayMode::Windowed);

	if (event.GetKey() == KEY_Q)
		m_GameWindow->SetWindowState(Light::WindowState::Visible);
	if (event.GetKey() == KEY_W)
		m_GameWindow->SetWindowState(Light::WindowState::Hidden);
	if (event.GetKey() == KEY_E)
		m_GameWindow->SetWindowState(Light::WindowState::Minimized);

	if (event.GetKey() == KEY_ESCAPE)
		m_GameWindow->Close();

	return true;
}

bool DemoLayer::OnMouseScroll(Light::MouseScrolledEvent& event)
{
	// this is just an example of LT_ASSERT, mouse wheel offset can be 0
	LT_ASSERT(event.GetOffset(), EC_CLIENT_ASSERTION, "Mouse wheel offset is 0"); 

	return true;
}

bool DemoLayer::OnWindowResize(Light::WindowResizedEvent& event)
{
	LT_INFO(event.GetLogInfo());
	return true;
}