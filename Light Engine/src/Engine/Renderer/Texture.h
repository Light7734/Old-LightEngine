#pragma once

#include "Core/Core.h"

#include <unordered_map>

namespace Light {

	struct TextureData
	{
		unsigned char* pixels;
		int width, height, channels;

		TextureData(): pixels(nullptr), width(0), height(0), channels(0) {}
		TextureData(const TextureData&) = delete;
		TextureData& operator=(const TextureData&) = delete;

		~TextureData() { free(pixels); }

		inline operator bool() const { return pixels && width && height && channels; }
	};

	struct TextureCoordinates
	{
		float xMin, yMin, xMax, yMax, sliceIndex;
	};

	class TextureAtlas
	{
	protected:
		std::unordered_map<std::string, TextureCoordinates> m_Segments;
		unsigned int m_Index;

		std::string m_Name;
	public:
		TextureAtlas(const std::string& name, const std::string& path, unsigned int sliceIndex);

		inline TextureCoordinates* GetCoordinates(const std::string& name) { return &m_Segments[name]; }

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

		static std::shared_ptr<TextureArray> Create(unsigned int slices);

		inline std::shared_ptr<TextureAtlas> GetAtlas(const std::string& name) { return m_Atlases[name]; }

		virtual void CreateAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath) = 0;
		virtual void DeleteAtlas(const std::string& name) = 0;

		virtual void Bind() = 0;
	};

}