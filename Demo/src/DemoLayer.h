#pragma once

#include <LightEngine.h>

class DemoLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::Window> m_GameWindow;
public:
	void OnAttach() override;
	void OnDetatch() override;

	void OnUpdate(float deltaTime) override;
	void OnRender() override;

	void OnEvent(Light::Event& event);
private:
	bool OnKeyPress(Light::KeyboardKeyPressedEvent& event);
	bool OnMouseScroll(Light::MouseScrolledEvent& event);
	bool OnWindowResize(Light::WindowResizedEvent& event);
};