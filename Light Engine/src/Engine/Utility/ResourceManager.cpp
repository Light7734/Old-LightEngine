#include "ltpch.h"
#include "ResourceManager.h"

#include "Renderer/Shader.h"

namespace Light {

	std::shared_ptr<TextureArray> ResourceManager::s_TextureArray;
	std::shared_ptr<TextureArray> ResourceManager::s_FontGlyphs;

	std::unordered_map<std::string, std::shared_ptr<Font>> ResourceManager::s_Fonts;

	void ResourceManager::LoadTexture(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		s_TextureArray->LoadTexture(name, texturePath, atlasPath);
	}

	void ResourceManager::LoadTexture(const std::string& name, const std::string& texturePath)
	{
		s_TextureArray->LoadTexture(name, texturePath);
	}

	void ResourceManager::LoadFont(const std::string& name, const std::string& path, unsigned int size)
	{
		s_FontGlyphs->LoadTexture(name, 1024u, 1024u);
		s_FontGlyphs->ResolveTextures();
		s_Fonts[name] = std::make_shared<Font>(s_FontGlyphs, s_FontGlyphs->GetTexture(name)->GetSliceIndex(), path, size);
	}

	void ResourceManager::ResolveTextures()
	{
		s_TextureArray->ResolveTextures();
	}

	void ResourceManager::DeleteTexture(const std::string& name)
	{
		// s_TextureArray->DeleteSlice(name);
	}

	void ResourceManager::DeleteFont(const std::string& name)
	{
		// s_FontGlyphs->DeleteSlice(name);
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