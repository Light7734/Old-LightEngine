#include "ltpch.h"
#include "glBlender.h"

#include <glad/glad.h>

namespace Light {

	glBlender::glBlender()
		: m_BlendFactorsMap({ { BlendFactor::ZERO, GL_ZERO },
		                      { BlendFactor::ONE , GL_ONE  },
		                      
		                      { BlendFactor::SRC_COLOR        , GL_SRC_COLOR           },
		                      { BlendFactor::SRC_COLOR_INVERSE, GL_ONE_MINUS_SRC_COLOR },
		                      { BlendFactor::SRC_ALPHA        , GL_SRC_ALPHA           },
		                      { BlendFactor::SRC_ALPHA_INVERSE, GL_ONE_MINUS_SRC_ALPHA },
		                      
		                      { BlendFactor::DST_COLOR        , GL_DST_COLOR           },
		                      { BlendFactor::DST_COLOR_INVERSE, GL_ONE_MINUS_DST_COLOR },
		                      { BlendFactor::DST_ALPHA        , GL_DST_ALPHA           },
		                      { BlendFactor::DST_ALPHA_INVERSE, GL_ONE_MINUS_DST_ALPHA } }),

		m_SrcFactor(BlendFactor::SRC_ALPHA),
		m_DstFactor(BlendFactor::SRC_ALPHA_INVERSE)
	{
		LT_PROFILE_FUNC();

		glBlendFunc(m_BlendFactorsMap.at(m_SrcFactor), m_BlendFactorsMap.at(m_DstFactor));
	}

	glBlender::~glBlender()
	{
		Disable();
	}

	void glBlender::Enable()
	{
		glEnable(GL_BLEND);
	}

	void glBlender::Disable()
	{
		glDisable(GL_BLEND);
	}

	void glBlender::SetSrcFactor(BlendFactor factor)
	{
		m_SrcFactor = factor;
		glBlendFunc(m_BlendFactorsMap.at(factor), m_BlendFactorsMap.at(m_DstFactor));
	}

	void glBlender::SetDstFactor(BlendFactor factor)
	{
		m_DstFactor = factor;
		glBlendFunc(m_BlendFactorsMap.at(m_SrcFactor), m_BlendFactorsMap.at(factor));
	}

}