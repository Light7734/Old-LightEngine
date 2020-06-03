#include "ltpch.h"
#include "FIleManager.h"

#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Light {

	FT_LibraryRec_* FileManager::s_Library = nullptr;
	FT_FaceRec_* FileManager::s_Face = nullptr;

	FontFileData::FontFileData(FT_LibraryRec_* library, const char* path, unsigned int size)
	{
		LT_CORE_ASSERT(!FT_New_Face(library, path, 0l, &face), "FontFileData::FontFileData: FT_New_Face failed");
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(face, 0u, size), "FontFileData::FontFileData: FT_Set_Pixel_Sizes failed");
	}

	FontFileData::~FontFileData()
	{
		FT_Done_Face(face);
	}

	void FontFileData::SetSize(unsigned int size)
	{
		LT_CORE_ASSERT(!FT_Set_Pixel_Sizes(face, 0u, size), "FontFileData::FontFileData: FT_Set_Pixel_Sizes failed");
	}

	void FontFileData::LoadChar(unsigned char charCode)
	{
		LT_CORE_ASSERT(face, "FontFileData::LoadChar: no fonts are loaded");
		LT_CORE_ASSERT(!FT_Load_Char(face, charCode, FT_LOAD_RENDER), "FontFileData::LoadChar: FT_Load_Char failed: {}", (char)charCode);
	}

	const glm::vec2& FontFileData::GetSize() const
	{
		return glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	}

	const glm::vec2& FontFileData::GetBearing() const
	{
		return glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	}

	unsigned int FontFileData::GetAdvance() const
	{
		return face->glyph->advance.x >> 6;
	}

	unsigned char* FontFileData::GetBuffer() const
	{
		return face->glyph->bitmap.buffer;
	}

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

	TextureFileData FileManager::LoadTextureFile(const std::string& path)
	{
		LT_PROFILE_FUNC();

		int x = 0, y = 0, channels = 0;
		unsigned char* pixels = stbi_load(path.c_str(), &x, &y, &channels, 4);

		if (!pixels)
			LT_CORE_ERROR("FileManager::LoadTextureFile: failed to load texture file: {}", path);

		return { pixels, x, y, channels };
	}

	FontFileData FileManager::LoadFont(const std::string& path, unsigned int size)
	{
		if (!s_Library)
			LT_CORE_ASSERT(!FT_Init_FreeType(&s_Library), "FileManager::LoadFont: FT_Init_FreeType failed");

		return FontFileData(s_Library, path.c_str(), size);
	}

}