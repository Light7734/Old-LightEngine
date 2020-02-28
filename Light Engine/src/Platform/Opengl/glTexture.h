#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

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