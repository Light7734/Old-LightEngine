#include "AudioLayer.h"

AudioLayer::AudioLayer()
{
	m_LayeDebugrName = "AudioLayer";

	Light::AudioEngine::Get().LoadAudio2D("getout", "res/getout.ogg", true);
	m_Music = Light::AudioEngine::Get().GetAudio("getout");
}

void AudioLayer::OnAttach()
{
	LT_TRACE("Attached Layer: {}", m_LayeDebugrName);
}

void AudioLayer::OnDetach()
{
	LT_TRACE("Detached Layer: {}", m_LayeDebugrName);
}

void AudioLayer::ShowDebugWindow()
{
	float volume = Light::AudioEngine::Get().GetMasterVolume();
	ImGui::SliderFloat("Master volume", &volume, 0.0f, 1.0f);
	Light::AudioEngine::Get().SetMasterVolume(volume);


	ImGui::BulletText("Music: getout [ %s ]", m_Music->getIsPaused() ? "paused" : "playing");
	if (ImGui::Button("Play"))
		m_Music->setIsPaused(false);
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
		m_Music->setIsPaused(true);


	ImGui::PushID("Bell");
	ImGui::BulletText("Bell"); ImGui::SameLine();
	if (ImGui::Button("Play"))
		Light::AudioEngine::Get().PlayAudio2D("res/bell.wav");
	ImGui::PopID();


	ImGui::PushID("Explosion");
	ImGui::BulletText("Explosion"); ImGui::SameLine();
	if (ImGui::Button("Play"))
		Light::AudioEngine::Get().PlayAudio2D("res/explosion.wav");
	ImGui::PopID();
}
