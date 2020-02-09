#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

#include <map>

namespace Light {

	struct TextureData
	{
		unsigned char* pixels;
		int width, height, channels;

		~TextureData();
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

		unsigned int m_Index;

		std::map<std::string, TextureCoordinates> m_Segments;

		TextureAtlas();
	public:
		static std::shared_ptr<TextureAtlas> Create(const std::string& atlasPath);
		~TextureAtlas();

		static void DestroyTextureArray();

		TextureCoordinates* GetCoordinates(const std::string& name) { return &m_Segments[name]; }
	protected:
		void ParseSegments(const std::string& data);
	};

}