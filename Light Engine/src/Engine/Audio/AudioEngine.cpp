#include "ltpch.h"
#include "AudioManager.h"

namespace Light {

	using namespace irrklang;

	AudioEngine::AudioEngine()
	{
		m_Engine = createIrrKlangDevice();
	}

	AudioEngine::~AudioEngine()
	{
		m_Engine->drop();
	}

	AudioEngine& AudioEngine::Get()
	{
		static AudioEngine engine;
		return engine;
	}

	void AudioEngine::PlayAudio2D(const char* path)
	{
		m_Engine->play2D(path);
	}

	void AudioEngine::PlayAudio3D(const char* path, const glm::vec3& position)
	{
		m_Engine->play3D(path, vec3df(position.x, position.y, position.z));
	}

	void AudioEngine::LoadAudio2D(const char* name, const char* path, bool loops /*= false*/, bool startPaused /*= true*/)
	{
		m_Audios[name] = m_Engine->play2D(path, loops, startPaused, true);
	}

	void AudioEngine::LoadAudio3D(const char* name, const char* path, const glm::vec3& position, bool loops /*= false*/, bool startPaused /*= true*/)
	{
		m_Audios[name] = m_Engine->play3D(path, vec3df(position.x, position.y, position.z), loops, startPaused, true);
	}

	void AudioEngine::DestroyAudio(const char* name)
	{
		if (m_Audios[name])
			m_Audios[name]->drop();
		else
			LT_CORE_ERROR("AudioEngine::DestroyAudio: audio name does not exists: {}", name);

		m_Audios.erase(name);
	}

	void AudioEngine::StopAllSounds()
	{
		m_Engine->stopAllSounds();
	}

	void AudioEngine::SetAllSoundsPaused(bool paused /*= true*/)
	{
		m_Engine->setAllSoundsPaused(paused);
	}

	void AudioEngine::SetMasterVolume(float volume)
	{
		m_Engine->setSoundVolume(volume);
	}

	float AudioEngine::GetMasterVolume()
	{
		return m_Engine->getSoundVolume();
	}

}