#include "ltpch.h"
#include "glShader.h"

#include "Utility/FileManager.h"

#include <glad/glad.h>

namespace Light {

	glShader::glShader(const std::string& vertex_source, const std::string& pixel_source)
	{
		m_ShaderID = glCreateProgram();

		const char* lvs_source = vertex_source.c_str();
		const char* lfs_source = pixel_source.c_str();

		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &lvs_source, nullptr);
		glCompileShader(vs);

		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &lfs_source, nullptr);
		glCompileShader(fs);

		glAttachShader(m_ShaderID, vs);
		glAttachShader(m_ShaderID, fs);
		glLinkProgram(m_ShaderID);

		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &m_InitStatus);
		if (!m_InitStatus)
		{
			LT_CORE_ERROR("Failed to link glShaders:");
			LT_CORE_ERROR("vs:\n{}", vertex_source);
			LT_CORE_ERROR("fs:\n{}", pixel_source);
		}
	}

	glShader::~glShader()
	{
		glDeleteShader(m_ShaderID);
	}

	void glShader::Bind()
	{
		glUseProgram(m_ShaderID);
	}

}