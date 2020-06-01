#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <bitset>
#include <set>

namespace Light {

	struct TextureImageData
	{
		unsigned char* pixels;
		int width, height, channels;

		TextureImageData() : pixels(nullptr), width(0), height(0), channels(0) {}

		TextureImageData(unsigned char* pixels_, int width_, int height_, int channels_)
		                 : pixels(pixels_), width(width_), height(height_), channels(channels_) {}
			
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

	struct TextureCoordinates
	{
		float xMin, yMin, xMax, yMax, sliceIndex;

		TextureCoordinates() = default;

		TextureCoordinates(float xMin_, float yMin_, float xMax_, float yMax_, float index) : xMin(xMin_), xMax(xMax_), yMin(yMin_), yMax(yMax_), sliceIndex(index) {}

		bool Contains(const TextureCoordinates& other) const
		{
			return sliceIndex == other.sliceIndex &&
				   xMin >= other.xMin &&
			       xMax <= other.xMax &&
			       yMin >= other.yMin &&
			       yMax <= other.yMax;
		}

		bool Intersects(const TextureCoordinates& other) const
		{
			return sliceIndex == other.sliceIndex && 
			       (xMax >= other.xMin) && (other.xMax >= xMin) &&
			       (yMax >= other.yMin) && (other.yMax >= yMin);
		}

		inline float GetWidth() const { return xMax - xMin; }
		inline float GetHeight() const { return yMax - yMin; }

		bool operator==(TextureCoordinates other) 
		{
			return xMin == other.xMin && yMin == other.yMin && xMax == other.xMax && yMax == other.yMax && sliceIndex == other.sliceIndex;
		}
	};

	class Texture
	{
	protected:
		std::unordered_map<std::string, TextureCoordinates> m_SubTextures;
		TextureCoordinates m_OccupiedSpace;
		TextureCoordinates m_TextureUV;
	public:
		Texture(const std::string& atlasPath, const TextureCoordinates& texture, const TextureCoordinates& slice);
		Texture(const TextureCoordinates& texture, const TextureCoordinates& slice);

		inline TextureCoordinates* GetSubTextureUV(const std::string& name) { return &m_SubTextures[name]; }
		inline TextureCoordinates* GetTextureUV() { return &m_TextureUV; }
		inline TextureCoordinates* GetOccupiedSpace() { return &m_OccupiedSpace; }

		inline unsigned int GetWidth() const { return m_OccupiedSpace.xMax - m_OccupiedSpace.xMin; }
		inline unsigned int GetHeight() const { return m_OccupiedSpace.yMax - m_OccupiedSpace.yMin; }
		inline unsigned int GetSliceIndex() const { return m_OccupiedSpace.sliceIndex; }
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
		std::vector<TextureCoordinates> m_OccupiedSpace;
	public:
		TextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);
		virtual ~TextureArray() = default;

		static std::shared_ptr<TextureArray> Create(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels = 4);

		void LoadTextureAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath);
		void LoadTexture(const std::string& name, const std::string& texturePath);
		void AllocateTexture(const std::string& name, unsigned width, unsigned int height);

		void ResolveTextures();

		void DeleteTexture(const std::string& name);

		virtual void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) = 0;
		virtual void UpdateSubTexture(const TextureCoordinates& uv, void* pixels) = 0;

		virtual void GenerateMips() = 0;

		virtual void Bind(unsigned int slot = 0) = 0;
			
		// getters
		inline std::shared_ptr<Texture> GetTexture(const std::string& name) { return m_Textures[name]; }

		inline const glm::vec2& GetSize() const { return { m_Width, m_Height }; }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
	};

}