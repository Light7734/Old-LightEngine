#pragma once

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

#include "Utility/FileManager.h"

namespace Light {

	class glTexture : public Texture
	{
	private:
		unsigned int m_TextureID = 0;
	public:
		glTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
		~glTexture();

		void Bind(unsigned int index = 0) override;
	};

}