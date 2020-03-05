#pragma once

#include <LightEngine.h>

class QuadsLayer;

class DemoLayer : public Light::Layer
{
private:	
	QuadsLayer* m_QuadsLayer;
public:
	DemoLayer();

	void OnAttach() override;
	void OnDetatch() override;

	void OnUserInterfaceUpdate() override;

	void OnEvent(Light::Event& event);
private:
	bool OnKeyPress(Light::KeyboardKeyPressedEvent& event);
};