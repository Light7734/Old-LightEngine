#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

#include <map>

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
		float xMin, yMin, xMax, yMax, atlasIndex;
	};

	class TextureAtlas
	{
	private:
		static std::vector<unsigned int> s_AvailableSlots;
	protected:
		static unsigned int s_Width, s_Height;
	protected:
		std::map<std::string, TextureCoordinates> m_Segments;
		unsigned int m_Index;
	protected:
		TextureAtlas();
	public:
		static std::shared_ptr<TextureAtlas> Create(const std::string& atlasPath);
		virtual ~TextureAtlas();

		TextureCoordinates* GetCoordinates(const std::string& name) { return &m_Segments[name]; }

		static void DestroyTextureArray();
	protected:
		void ParseSegments(const std::string& data);
	};

}