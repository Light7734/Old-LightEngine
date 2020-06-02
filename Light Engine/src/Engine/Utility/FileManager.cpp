#include "ltpch.h"
#include "FIleManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Light {

	FT_LibraryRec_* FileManager::s_Library = nullptr;
	FT_FaceRec_* FileManager::s_Face = nullptr;

	std::string FileManager::LoadTextFile(const std::string& path)
	{
		LT_PROFILE_FUNC();

		std::fstream stream(path);
		if (!stream.is_open())
		{
			LT_CORE_ERROR("FileManager::LoadTextFile: failed to load text file: {}", path);
			return "";
		}

		std::string line;
		std::stringstream ss;
		while (std::getline(stream, line))
			ss << line << '\n';

		return ss.str();
	}

	TextureImageData FileManager::LoadTextureFile(const std::string& path)
	{
		LT_PROFILE_FUNC();

		int x = 0, y = 0, channels = 0;
		unsigned char* pixels = stbi_load(path.c_str(), &x, &y, &channels, 4);

		if (!pixels)
			LT_CORE_ERROR("FileManager::LoadTextureFile: failed to load texture file: {}", path);

		return { pixels, x, y, channels };
	}

	void FileManager::FreeTextureFile(unsigned char* pixels)
	{
		stbi_image_free(pixels);
	}

	void FileManager::LoadFont(const std::string& path, unsigned int size)
	{
		if (!s_Library)
			LT_CORE_ASSERT(!FT_Init_FreeType(&s_Library), "FileManager::LoadFont: FT_Init_FreeType failed");

		if (s_Face)
		{
			LT_CORE_WARN("FileManager::LoadFont: FreeLoadedFont was not called after the previous LoadFont");
		}

		LT_CORE_ASSERT(!FT_New_Face(s_Library, path.c_str(), 0, &s_Face), "FileManager::LoadFont: FT_New_Face failed");
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(s_Face, 0, size), "FileManager::LoadFont: FT_Set_Pixel_Sizes failed");
	}

	const FontCharGlyphData& FileManager::LoadFontCharGlyph(unsigned char charCode, unsigned char** outBuffer)
	{
		LT_CORE_ASSERT(s_Face, "FileManager::LoadFontChar: no fonts are loaded");
		LT_CORE_ASSERT(!FT_Load_Char(s_Face, charCode, FT_LOAD_RENDER), "FileManager::LoadFontChar: FT_Load_Char failed: {}", (char)charCode);

		*outBuffer = s_Face->glyph->bitmap.buffer;

		return FontCharGlyphData(glm::vec2(s_Face->glyph->bitmap.width, s_Face->glyph->bitmap.rows),
		                         glm::vec2(s_Face->glyph->bitmap_left, s_Face->glyph->bitmap_top),
		                         (unsigned int)s_Face->glyph->advance.x >> 6);
	}

	void FileManager::FreeLoadedFont()
	{
		FT_Done_Face(s_Face);
		s_Face = nullptr;
	}

}