#pragma once

#include <LightEngine.h>

class QuadsLayer;
class PostProcessLayer;

class DemoLayer : public Light::Layer
{
private:	
	QuadsLayer* m_QuadsLayer;
	PostProcessLayer* m_PostProcessLayer;

	std::shared_ptr<Light::Camera> m_Camera;
	float m_CameraSpeed;
public:
	DemoLayer();

	void OnAttach() override;
	void OnDetatch() override;

	void OnUpdate(float DeltaTime) override;

	void OnUserInterfaceUpdate() override;

	void OnEvent(Light::Event& event) override;
private:
	bool OnKeyPress(Light::KeyboardKeyPressedEvent& event);
	bool OnMouseScroll(Light::MouseScrolledEvent& event);
	bool OnWindowResize(Light::WindowResizedEvent& event);
};