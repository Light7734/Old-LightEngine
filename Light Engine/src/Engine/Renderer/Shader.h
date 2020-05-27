#pragma once

#include "VertexLayout.h"

#include "Core/Core.h"

namespace Light {

	enum TextureBindingSlot
	{
		BINDING_TEXTUREARRAY0,
		BINDING_FONTGLYPHARRAY0,
		BINDING_FRAMEBUFFER0, 
		BINDING_FRAMEBUFFER1, 
		BINDING_FRAMEBUFFER2, 
	};

	class Shader
	{
	protected:
		std::vector<std::pair<std::string, VertexElementType>> m_Elements;
	public:
		virtual ~Shader() = default;

		static std::shared_ptr<Shader> Create(const std::string& vertex, const std::string& fragment);

		virtual void Bind() = 0;

		inline const std::vector<std::pair<std::string, VertexElementType>>& GetElements() const { return m_Elements; }
	private:
		static void ExtractShaderSource(std::string& src, const std::string& delim);
	};

}