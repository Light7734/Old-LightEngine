#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Light {

	class glTextureArray : public TextureArray
	{
	private:
		unsigned int m_ArrayID;

		unsigned int m_Format;
		unsigned int m_Channels;
	public:
		glTextureArray(unsigned int slices, unsigned int channels);
		~glTextureArray();

		// create
		void CreateSliceWithAtlas(const std::string& texturePath, const std::string& atlasName, const std::string& atlasPath) override;

		unsigned int CreateSlice(const std::string& texturePath) override;

		unsigned int CreateSlice(unsigned int width, unsigned int height, void* pixels) override;

		// delete
		void DeleteSliceWithAtlas(const std::string& atlasName) override;

		void DeleteSlice(unsigned int sliceIndex) override;

		// write
		void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) override;
		void GenerateMips() override;

		void Bind() override;
	};

}