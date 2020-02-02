#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

#define LT_MAXIMUM_TEXTURE_SLOTS 16 // #todo

namespace Light {

	struct TextureData
	{
		unsigned char* pixels;
		int width, height, channels;
	public:
		~TextureData();

		template<class ...Args>
		static std::unique_ptr<TextureData> Create(Args&& ...args)
		{
			return std::unique_ptr<TextureData>(new TextureData(std::forward<Args>(args)...));
		}
	private:
		TextureData(unsigned char* _pixels, int _width, int _height, int _channels)
			: pixels(_pixels), width(_width), height(_height), channels(_channels) {}

		TextureData           (const TextureData&) = delete;
		TextureData& operator=(const TextureData&) = delete;

	};

	struct TextureCoordinates
	{
		float xMin, yMin, xMax, yMax;
	};

	class Texture
	{
	protected:
		unsigned unsigned int m_BoundSlot = 0;
		bool b_Bound = false;
	public:
		static std::shared_ptr<Texture> Create(const std::string& path);
		static std::shared_ptr<Texture> Create(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
		
		inline bool IsBonud() const { return b_Bound; }

		virtual void Bind(unsigned int = 0) = 0;
		inline  void UnBind() { b_Bound = false; }

		inline unsigned int GetIndex() { return m_BoundSlot; }
	};

}