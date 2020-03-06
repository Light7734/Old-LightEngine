#pragma once

#include "Core/Core.h"

#include "Renderer/Blender.h"

#include <unordered_map>

namespace Light {

	class glBlender : public Blender
	{
	private:
		const std::unordered_map<BlendFactor, int> m_BlendFactorsMap;
	public:
		glBlender();

		void EnableImpl() override;
		void DisableImpl() override;

		void SetSrcFactorImpl(BlendFactor factor) override;
		void SetDstFactorImpl(BlendFactor factor) override;
	};

}