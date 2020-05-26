#pragma once

#include "Core/Core.h"

#include "Shader.h"

namespace Light {

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		static std::shared_ptr<Framebuffer> Create(const std::string& vertex, const std::string& fragment, TextureBindingSlot slot = BINDING_FRAMEBUFFER0);

		virtual void BindAsTarget() = 0;
		virtual void BindAsResource() = 0;

		virtual void Resize(unsigned int width, unsigned int height) = 0;
	};

}