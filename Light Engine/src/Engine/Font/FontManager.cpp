#include "ltpch.h"
#include "FontManager.h"

namespace Light {

	std::unordered_map<std::string, std::shared_ptr<Font>> FontManager::m_Fonts;

	std::shared_ptr<TextureArray> FontManager::m_TextureArray;

	void FontManager::LoadFont(const std::string& name, const std::string& path, unsigned int size)
	{
		m_Fonts[name] = std::make_shared<Font>(m_TextureArray, path, size);
	}

	void FontManager::RemoveFont(const std::string& name)
	{
		m_Fonts.erase(name);
	}

	void FontManager::Init()
	{
		m_Fonts.clear();
		m_TextureArray.reset();

		m_TextureArray = TextureArray::Create(16u, 1u);
	}

}