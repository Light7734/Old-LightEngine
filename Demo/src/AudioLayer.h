#pragma once

#include <LightEngine.h>

class AudioLayer : public Light::Layer 
{
private:
	// Audio is just a typedef of irrklang::ISound, you should check irrklang's documentations for better understanding of AudioEngine.
	Light::Audio* m_Music;
public:
	AudioLayer();
	~AudioLayer();

	void OnAttach();
	void OnDetach();

	void ShowDebugWindow() override;
};