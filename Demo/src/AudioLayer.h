#pragma once

#include <LightEngine.h>

class AudioLayer : public Light::Layer 
{
private:
	Light::Audio* m_Music;
public:
	AudioLayer();

	void OnAttach();
	void OnDetach();

	void ShowDebugWindow() override;

};