#pragma once

#include "Core/Core.h"

#include "Renderer/Blender.h"

#include <unordered_map>

namespace Light {

	class glBlender : public Blender
	{
	private:
		const std::unordered_map<BlendFactor, int> m_BlendFactorsMap;

		BlendFactor m_SrcFactor;
		BlendFactor m_DstFactor;
	public:
		glBlender();
		~glBlender();

		void Enable() override;
		void Disable() override;

		void SetSrcFactor(BlendFactor factor) override;
		void SetDstFactor(BlendFactor factor) override;
	};

}