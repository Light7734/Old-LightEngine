#include "AudioLayer.h"

AudioLayer::AudioLayer()
{
	m_LayeDebugrName = "AudioLayer";
	m_Music = Light::AudioEngine::Get().PlayAudio2D("res/getout.ogg", true, true);
}

void AudioLayer::OnAttach()
{
	LT_TRACE("Attached: {}", m_LayeDebugrName);
}

void AudioLayer::OnDetach()
{
	LT_TRACE("Detached: {}", m_LayeDebugrName);
}

void AudioLayer::ShowDebugWindow()
{
	float volume = Light::AudioEngine::Get().GetMasterVolume();
	ImGui::SliderFloat("Master volume", &volume, 0.0f, 1.0f);
	Light::AudioEngine::Get().SetMasterVolume(volume);


	ImGui::BulletText("Music: getout");
	ImGui::SameLine();
	if (ImGui::Button(m_Music->getIsPaused() ? "resume" : "pause"))
		m_Music->setIsPaused(!m_Music->getIsPaused());


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
