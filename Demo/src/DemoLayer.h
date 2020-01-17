#pragma once

#include <LightEngine.h>

#include "QuadLayer.h"

class DemoLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::Window> m_GameWindow;
	QuadLayer* m_Quad;
public:
	void OnAttach() override;
	void OnDetatch() override;

	void OnUpdate(float deltaTime) override;

	void OnEvent(Light::Event& event);
private:
	bool OnKeyPress(Light::KeyboardKeyPressedEvent& event);
};