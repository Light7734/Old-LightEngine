#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Light {

	class glTextureArray : public TextureArray 
	{
	private:
		unsigned int m_TextureArrayID;
		unsigned int m_Format;
	public:
		glTextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);
		~glTextureArray();

		void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) override;

		void GenerateMips() override;

		void Bind(unsigned int slot = 0) override;
	};

}