#pragma once

#include <LightEngine.h>

#include "TriangleLayer.h"

class DemoLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::Window> m_GameWindow;
	std::shared_ptr<TriangleLayer> m_TriangleLayer;
public:
	void OnAttach() override;
	void OnDetatch() override;

	void OnUpdate(float deltaTime) override;

	void OnEvent(Light::Event& event);
private:
	bool OnKeyPress(Light::KeyboardKeyPressedEvent& event);
};