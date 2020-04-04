#pragma once

#include "Core/Core.h"

#include <irrKlang.h>

#include <glm/glm.hpp>

namespace Light{

	typedef irrklang::ISound Audio;

	class AudioEngine
	{
	private:
		irrklang::ISoundEngine* m_Engine;
		std::unordered_map<std::string, Audio*> m_Audios;
	private:
		AudioEngine();
	public:
		~AudioEngine();

		static AudioEngine& Get();

		void PlayAudio2D(const char* path);
		void PlayAudio3D(const char* path, const glm::vec3& position);

		void LoadAudio2D(const char* name, const char* path, bool loops = false, bool startPaused = true);
		void LoadAudio3D(const char* name, const char* path, const glm::vec3& position, bool loops = false, bool startPaused = true);

		void DestroyAudio(const char* name);

		void StopAllSounds();
		void SetAllSoundsPaused(bool paused = true);

		void SetMasterVolume(float volume);
		float GetMasterVolume();

		inline Audio* GetAudio(const char* name) { return m_Audios[name]; }
	};

}