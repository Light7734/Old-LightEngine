#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Light {

	struct FontCharGlyphData
	{
		glm::vec2 size;
		glm::vec2 bearing;
		unsigned int advance;

		FontCharGlyphData(const glm::vec2& size_, const glm::vec2& bearing_, unsigned int advance_)
			: size(size_), bearing(bearing_), advance(advance_) {}
	};

	class FileManager
	{
	private:
		static FT_LibraryRec_* s_Library;
		static FT_FaceRec_* s_Face;
	public:
		FileManager() = delete;

		static std::string LoadTextFile(const std::string& path);

		static TextureImageData LoadTextureFile(const std::string& path);

		static void FreeTextureFile(unsigned char* pixels);

		static void LoadFont(const std::string& path, unsigned int size);
		static const FontCharGlyphData& LoadFontCharGlyph(unsigned char charCode, unsigned char** outBuffer);
		
		static void FreeLoadedFont();
	};

}