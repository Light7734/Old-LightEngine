#pragma once

#include "Core/Core.h"

#include <irrKlang.h>

#include <glm/glm.hpp>

namespace Light{

	typedef irrklang::ISoundSource AudioSource;
	typedef irrklang::ISound Audio;

	class AudioEngine
	{
	private:
		irrklang::ISoundEngine* m_Engine;
		std::unordered_map<std::string, AudioSource*> m_AudioSources;
	private:
		AudioEngine();
	public:
		~AudioEngine();

		static AudioEngine& Get();

		Audio* PlayAudio2D(const char* path, bool playLooped = false, bool startPaused = false, bool track = false);
		Audio* PlayAudio3D(const char* path, const glm::vec3& position, bool playLooped = false, bool startPaused = false, bool track = false);

		Audio* PlayAudio2D(AudioSource* source, bool playLooped = false, bool startPaused = false, bool track = false);
		Audio* PlayAudio3D(AudioSource* source, const glm::vec3& position, bool playLooped = false, bool startPaused = false, bool track = false);

		AudioSource* LoadAudio(const char* name, const char* path);

		void DeleteAudio(const char* name);

		void SetMasterVolume(float volume);

		void StopAllSounds();

		void SetAllSoundsPaused(bool paused = true);

		float GetMasterVolume();

		AudioSource* GetAudio(const char* name);
	};

}