#include "ltpch.h"
#include "ResourceManager.h"

namespace Light {

	std::shared_ptr<TextureArray> ResourceManager::s_TextureArray;
	std::shared_ptr<TextureArray> ResourceManager::s_FontGlyphs;

	std::unordered_map<std::string, std::shared_ptr<Font>> ResourceManager::s_Fonts;

	void ResourceManager::LoadTexture(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		s_TextureArray->CreateSlice(name, texturePath, atlasPath);
	}

	void ResourceManager::DeleteTexture(const std::string& name)
	{
		s_TextureArray->DeleteSlice(name);
	}

	void ResourceManager::LoadFont(const std::string& name, const std::string& path, unsigned int size)
	{
		s_FontGlyphs->CreateSlice(name, s_FontGlyphs->GetWidth(), s_FontGlyphs->GetHeight());
		s_Fonts[name] = std::make_shared<Font>(s_FontGlyphs, s_FontGlyphs->GetTexture(name)->GetSliceIndex(), path, size);
	}

	void ResourceManager::DeleteFont(const std::string& name)
	{
		s_FontGlyphs->DeleteSlice(name);
		s_Fonts.erase(name);
	}

	void ResourceManager::Init()
	{
		s_Fonts.clear();

		s_TextureArray = TextureArray::Create(2048u, 2048u, 12u, 4u);
		s_FontGlyphs   = TextureArray::Create(2048u, 2048u, 4u, 1u);

		s_TextureArray->Bind(0u);
		s_FontGlyphs->Bind(1u);
	}

}