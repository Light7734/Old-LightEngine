#pragma once

#include "Core/Core.h"

#include <unordered_map>

namespace Light {

	struct TextureImageData
	{
		unsigned char* pixels;
		int width, height, channels;

		TextureImageData(unsigned char* pixels_, int width_, int height_, int channels_)
		                 : pixels(pixels_), width(width_), height(height_), channels(channels_) {}
		TextureImageData() : pixels(nullptr), width(0), height(0), channels(0) {}
		TextureImageData(const TextureImageData&) = delete;
		TextureImageData& operator=(const TextureImageData&) = delete;

		~TextureImageData() { free(pixels); }

		inline operator bool() const { return pixels && width && height && channels; }
	};

	struct SubTexture
	{
		float xMin, yMin, xMax, yMax, sliceIndex;
	};

	class TextureAtlas
	{
	protected:
		std::unordered_map<std::string, SubTexture> m_SubTextures;
		unsigned int m_Index;

		std::string m_Name;
	public:
		TextureAtlas(const std::string& name, const std::string& path, unsigned int sliceIndex);

		inline SubTexture* GetSubTexture(const std::string& name) { return &m_SubTextures[name]; }

		inline unsigned int GetSliceIndex() const { return m_Index; }

		inline const std::string& GetName() const { return m_Name; }
	private:
	};

	class TextureArray
	{
	protected:
		std::vector<unsigned int> m_AvailableSlots;	
		std::unordered_map<std::string, std::shared_ptr<TextureAtlas>> m_Atlases;

		unsigned int m_Width, m_Height, m_Depth;
	public:
		TextureArray() : m_Width(0), m_Height(0), m_Depth(0) {}

		virtual ~TextureArray() = default;

		static std::shared_ptr<TextureArray> Create(unsigned int slices, unsigned int channels = 4);

		virtual void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) = 0;

		virtual void GenerateMips() = 0;

		virtual void CreateSliceWithAtlas(const std::string& texturePath, const std::string& atlasName, const std::string& atlasPath) = 0;

		// #todo: I'm reading this bottom todo and have no idea what I've wrote there, figure out what to do later
		// // #todo: this is not very safe because we can accidentally pass sliceIndex of a SliceWithTexture
		virtual unsigned int CreateSlice(const std::string& texturePath) = 0;

		virtual unsigned int CreateSlice(unsigned int width, unsigned int height, void* pixels) = 0;

		virtual void DeleteSliceWithAtlas(const std::string& atlasName) = 0;

		virtual void DeleteSlice(unsigned int sliceIndex) = 0;

		virtual void Bind() = 0;
			
		inline std::shared_ptr<TextureAtlas> GetAtlas(const std::string& name) { return m_Atlases[name]; }
	};

}