#pragma once

#include "Core/Core.h"

#include "Renderer/Blender.h"

namespace Light {

	class glBlender : public Blender
	{
	public:
		glBlender();

		void EnableImpl() override;
		void DisableImpl() override;
	};

}