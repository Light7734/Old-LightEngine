#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp> 

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Light {

	struct TextureFileData
	{
		unsigned char* pixels;
		int width, height, channels;

		TextureFileData() : pixels(nullptr), width(0), height(0), channels(0) {}

		TextureFileData(unsigned char* pixels_, int width_, int height_, int channels_)
			: pixels(pixels_), width(width_), height(height_), channels(channels_) {}

		inline void Free() { free(pixels); pixels = nullptr; }

		inline operator bool() const { return pixels && width && height && channels; }

		inline bool operator<(const TextureFileData& other) const
		{
			return width * height < other.width* other.height;
		}

		inline bool operator>(const TextureFileData& other) const
		{
			return width * height > other.width* other.height;
		}
	};

	struct FontFileData
	{
	private:
		FT_FaceRec_* face;
	public:
		FontFileData(FT_LibraryRec_* library, const char* path, unsigned int size);
		~FontFileData();

		void LoadChar(unsigned char charCode);

		void SetSize(unsigned int size);

		const glm::vec2& GetSize() const;
		const glm::vec2& GetBearing() const;
		unsigned int GetAdvance() const;

		unsigned char* GetBuffer() const;
	};

	class FileManager
	{
	private:
		static FT_LibraryRec_* s_Library;
		static FT_FaceRec_* s_Face;
	public:
		FileManager() = delete;

		static std::string LoadTextFile(const std::string& path);

		static TextureFileData LoadTextureFile(const std::string& path);

		static FontFileData LoadFont(const std::string& path, unsigned int size);
	};

}