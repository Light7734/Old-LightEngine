#pragma once

#include "Core/Core.h"

namespace Light {

	class MSAA
	{
	public:
		static std::shared_ptr<MSAA> Create(unsigned int samples);

		virtual void BindFrameBuffer() = 0;

		virtual void Resolve() = 0;

		virtual void Resize(unsigned int width, unsigned int height) = 0;
	};

}