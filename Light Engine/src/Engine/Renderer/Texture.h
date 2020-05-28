#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <bitset>
#include <set>

namespace Light {

	struct TextureImageData
	{
		unsigned char* pixels;
		int width, height, channels;

		TextureImageData(unsigned char* pixels_, int width_, int height_, int channels_)
		                 : pixels(pixels_), width(width_), height(height_), channels(channels_) {}
		TextureImageData() : pixels(nullptr), width(0), height(0), channels(0) {}
			
		inline operator bool() const { return pixels && width && height && channels; }

		inline bool operator<(const TextureImageData& other)
		{
			return width * height < other.width* other.height;
		}

		inline bool operator>(const TextureImageData& other)
		{
			return width * height > other.width * other.height;
		}
	};

	struct SubTexture
	{
		float xMin, yMin, xMax, yMax, sliceIndex;

		SubTexture() = default;
		SubTexture(float xMin_, float yMin_, float xMax_, float yMax_, float index) : xMin(xMin_), xMax(xMax_), yMin(yMin_), yMax(yMax_), sliceIndex(index) {}
	};

	class Texture
	{
	protected:
		std::unordered_map<std::string, SubTexture> m_SubTextures;
		SubTexture m_Texture;
	public:
		Texture(const std::string& atlasPath, const SubTexture& texture, const SubTexture& slice);
		Texture(const SubTexture& texture);

		inline SubTexture* GetSubTexture(const std::string& name) { return &m_SubTextures[name]; }
		inline SubTexture* GetTexture() { return &m_Texture; }

		inline unsigned int GetSliceIndex() const { return m_Texture.sliceIndex; }
		inline unsigned int GetWidth() const { return m_Texture.xMax - m_Texture.xMin; }
		inline unsigned int GetHeight() const { return m_Texture.yMax - m_Texture.yMin; }
	};

	class TextureArray
	{
	protected:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

		unsigned int m_Width, m_Height, m_Depth, m_Channels;

		struct UnresolvedTextureData {
			std::string name;
			std::string atlasPath;
			TextureImageData texture;	

			inline bool operator>(const UnresolvedTextureData& other) { // to be used by std::sort
				return texture.width * texture.height > other.texture.width * other.texture.height;
			}
		};
		std::vector<UnresolvedTextureData> m_UnresolvedTextures;
		std::vector< std::array<std::bitset<2048>, 2048> > m_Pixels;
	public:
		TextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);
		virtual ~TextureArray() = default;

		static std::shared_ptr<TextureArray> Create(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels = 4);

		void LoadTexture(const std::string& name, const std::string& texturePath, const std::string& atlasPath);
		void LoadTexture(const std::string& name, const std::string& texturePath);
		void LoadTexture(const std::string& name, unsigned width, unsigned int height);

		void ResolveTextures();

		void DeleteTexture(const std::string& name);

		virtual void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) = 0;

		virtual void GenerateMips() = 0;

		virtual void Bind(unsigned int slot = 0) = 0;
			
		// getters
		inline std::shared_ptr<Texture> GetTexture(const std::string& name) { return m_Textures[name]; }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
	};

}