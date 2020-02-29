#include "ltpch.h"
#include "glBlender.h"

#include <glad/glad.h>

namespace Light {

	glBlender::glBlender()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void glBlender::EnableImpl()
	{
		b_Enabled = true;
		glEnable(GL_BLEND);
	}

	void glBlender::DisableImpl()
	{
		b_Enabled = false;
		glDisable(GL_BLEND);
	}

}