#include "ltpch.h"
#include "ResourceManager.h"

#include "Renderer/Shader.h"

namespace Light {

	std::shared_ptr<TextureArray> ResourceManager::s_TextureArray;
	std::shared_ptr<TextureArray> ResourceManager::s_FontGlyphs;

	std::unordered_map<std::string, std::shared_ptr<Font>> ResourceManager::s_Fonts;

	void ResourceManager::LoadTextureAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		s_TextureArray->LoadTextureAtlas(name, texturePath, atlasPath);
	}

	void ResourceManager::LoadTexture(const std::string& name, const std::string& texturePath)
	{
		s_TextureArray->LoadTexture(name, texturePath);
	}

	void ResourceManager::LoadFont(const std::string& name, const std::string& path, unsigned int size)
	{
		s_FontGlyphs->ResolveTextures();
		s_Fonts[name] = std::make_shared<Font>(name, path, s_FontGlyphs, size);
	}

	void ResourceManager::ResolveTextures()
	{
		s_TextureArray->ResolveTextures();
	}

	void ResourceManager::DeleteTexture(const std::string& name)
	{
		s_TextureArray->DeleteTexture(name);
	}

	void ResourceManager::DeleteFont(const std::string& name)
	{
		s_FontGlyphs->DeleteTexture(name);
		s_Fonts.erase(name);
	}

	void ResourceManager::Terminate()
	{
		s_TextureArray.reset();
		s_FontGlyphs.reset();
		s_Fonts.clear();
	}

	void ResourceManager::Init()
	{
		s_Fonts.clear();

		s_TextureArray = TextureArray::Create(2048u, 2048u, 16u, 4u);
		s_FontGlyphs   = TextureArray::Create(2048u, 2048u, 3u, 1u);

		s_TextureArray->Bind(BINDING_TEXTUREARRAY0);
		s_FontGlyphs->Bind(BINDING_FONTGLYPHARRAY0);
	}

}