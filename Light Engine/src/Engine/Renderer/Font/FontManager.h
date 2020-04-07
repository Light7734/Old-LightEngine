#pragma once

#include "Renderer/Font/Font.h"

#include "Core/Core.h"

#include "Renderer/Texture.h"

#include <map>

namespace Light {

	class FontManager
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<Font>> m_Fonts;
		static std::shared_ptr<TextureArray> m_TextureArray;
	public:
		static void LoadFont(const std::string& name, const std::string& path, unsigned int size);

		static void RemoveFont(const std::string& name);

		static inline void BindTextureArray() { m_TextureArray->Bind(); }

		static inline std::shared_ptr<Font> GetFont(const std::string& name) { return m_Fonts[name]; }
	private:
		friend class GraphicsContext;
		static void Init();
	};

}