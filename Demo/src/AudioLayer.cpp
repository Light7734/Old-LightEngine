#include "AudioLayer.h"

AudioLayer::AudioLayer()
{
	LT_PROFILE_FUNC();
	LT_TRACE("AudioLayer::AudioLayer");
	m_LayeDebugrName = "AudioLayer";

	// load the audio by playing it paused, playLooped is true.
	// if none of playLooped or startPaused or track is true, then PlayAudio2D plays the audio once and won't return anything.
	//     if PlayAudio2D does return Audio*, you should call m_Music->drop() otherwise it results in memory waste.
	m_Music = Light::AudioEngine::Get().PlayAudio2D("res/getout.ogg", true, true);
}

AudioLayer::~AudioLayer()
{
	m_Music->drop();
}

void AudioLayer::OnAttach()
{
	LT_TRACE("AudioLayer::OnAttach");
}

void AudioLayer::OnDetach()
{
	LT_TRACE("AudioLayer::OnDetach");
}

void AudioLayer::ShowDebugWindow()
{
	// master volume
	float volume = Light::AudioEngine::Get().GetMasterVolume();
	if(ImGui::SliderFloat("Master volume", &volume, 0.0f, 1.0f))
		Light::AudioEngine::Get().SetMasterVolume(volume);


	ImGui::BulletText("Music: getout");
	ImGui::SameLine();
	if (ImGui::Button(m_Music->getIsPaused() ? "resume" : "pause"))
		m_Music->setIsPaused(!m_Music->getIsPaused()); // pause / resume


	ImGui::PushID("Bell");
	ImGui::BulletText("Bell"); ImGui::SameLine();
	if (ImGui::Button("Play"))
		// play the bell.wav, nothing is set to true, so the audio gets played once, no need for .drop()
		Light::AudioEngine::Get().PlayAudio2D("res/bell.wav"); 
	ImGui::PopID();


	ImGui::PushID("Explosion");
	ImGui::BulletText("Explosion"); ImGui::SameLine();
	// play the explosion.wav, nothing is set to true, so the audio gets played once, no need for .drop()
	if (ImGui::Button("Play"))
		Light::AudioEngine::Get().PlayAudio2D("res/explosion.wav");
	ImGui::PopID();
}
