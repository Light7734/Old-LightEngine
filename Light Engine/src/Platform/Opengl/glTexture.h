#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Light {

	class glTextureArray : public TextureArray
	{
	private:
		unsigned int m_ArrayID;
	public:
		glTextureArray(unsigned int slices);
		~glTextureArray();

		void CreateAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath) override;
		void DeleteAtlas(const std::string& name) override;

		void Bind() override;
	};

}