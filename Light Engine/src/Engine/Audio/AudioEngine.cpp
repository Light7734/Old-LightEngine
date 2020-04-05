#include "ltpch.h"
#include "AudioEngine.h"

namespace Light {

	using namespace irrklang;

	AudioEngine::AudioEngine()
	{
		m_Engine = createIrrKlangDevice();
	}

	AudioEngine::~AudioEngine()
	{
		m_Engine->drop();

		for (const auto& element : m_AudioSources)
			element.second->drop();
	}

	AudioEngine& AudioEngine::Get()
	{
		static AudioEngine engine;
		return engine;
	}

	Audio* AudioEngine::PlayAudio2D(const char* path, bool playLooped, bool startPaused, bool track)
	{
		return m_Engine->play2D(path, playLooped, startPaused, track);
	}

	Audio* AudioEngine::PlayAudio2D(AudioSource* source, bool playLooped, bool startPaused, bool track)
	{
		return m_Engine->play2D(source, playLooped, startPaused, track);
	}

	Audio* AudioEngine::PlayAudio3D(const char* path, const glm::vec3& position, bool playLooped, bool startPaused, bool track)
	{
		return m_Engine->play3D(path, vec3df(position.x, position.y, position.z), playLooped, startPaused, track);
	}

	Audio* AudioEngine::PlayAudio3D(AudioSource* source, const glm::vec3& position, bool playLooped, bool startPaused, bool track)
	{
		return m_Engine->play3D(source, { position.x, position.y, position.z }, playLooped, startPaused, track);
	}

	AudioSource* AudioEngine::LoadAudio(const char* name, const char* path)
	{
		if (m_AudioSources[name])
			LT_CORE_WARN("AudioEngine::LoadAudio2D: overwriting Audio source: '{}'", name);

		return m_AudioSources[name] = m_Engine->addSoundSourceFromFile(path);
	}

	void AudioEngine::DeleteAudio(const char* name)
	{
		if (m_AudioSources[name])
			m_AudioSources[name]->drop();
		else
			LT_CORE_ERROR("AudioEngine::DestroyAudio: audio name does not exists: {}", name);

		m_AudioSources.erase(name);
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