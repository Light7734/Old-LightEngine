#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

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

		operator bool() const
		{
			return pixels && width && height && channels;
		}
	};

	struct TextureCoordinates
	{
		float xMin, yMin, xMax, yMax, sliceIndex;
	};

	class TextureAtlas
	{
	private:
		static std::vector<unsigned int> s_AvailableSlots;
	protected:
		static unsigned int s_Width, s_Height;

		std::unordered_map<std::string, TextureCoordinates> m_Segments;
		unsigned int m_Index;
	public:
		TextureAtlas();
		virtual ~TextureAtlas();

		static std::shared_ptr<TextureAtlas> Create(const std::string& atlasPath);

		static void DestroyTextureArray();

		TextureCoordinates* GetCoordinates(const std::string& name) { return &m_Segments[name]; }
	private:
		void ParseSegments(const std::string& data);
	};

}