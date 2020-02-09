#pragma once

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

#include "Utility/FileManager.h"

namespace Light {

	class glTextureAtlas : public TextureAtlas
	{
	private:
		static unsigned int m_AtlasID;
	public:
		glTextureAtlas(const TextureData& data);

		static void DestroyTextureArray();
	private:
		void Init(unsigned int width, unsigned int height);
	};

}