#pragma once

#include "Core/Core.h"

namespace Light {

	class TextureArray;
	class Texture;
	class Font;

	class ResourceManager
	{
	private:
		static std::shared_ptr<TextureArray> s_TextureArray;
		static std::shared_ptr<TextureArray> s_FontGlyphs;

		static std::unordered_map<std::string, std::shared_ptr<Font>> s_Fonts;
	public:
		static void LoadTextureAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath);
		static void LoadTexture(const std::string& name, const std::string& texturePath);
		static void LoadFont(const std::string& name, const std::string& path, unsigned int size);

		static void ResolveTextures();

		static void DeleteTexture(const std::string& name);
		static void DeleteFont(const std::string& name);

		static std::shared_ptr<Texture> GetTexture(const std::string& name);
		static std::shared_ptr<Font> GetFont(const std::string& name);
	private:
		friend class GraphicsContext;
		static void Terminate();
		static void Init();
	};

}