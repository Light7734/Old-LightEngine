#pragma once

#include <LightEngine.h>

class AudioLayer;
class PostProcessLayer;
class QuadsLayer;
class TextLayer;

class DemoLayer : public Light::Layer
{
private:	
	// layers
	AudioLayer* m_AudioLayer;
	PostProcessLayer* m_PostProcessLayer;
	QuadsLayer* m_QuadsLayer;
	TextLayer* m_TextLayer;

	// camera
	std::shared_ptr<Light::Camera> m_Camera;
	std::shared_ptr<Light::CameraController> m_CameraController;
	float m_CameraSpeed, m_CameraZoomSpeed;
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