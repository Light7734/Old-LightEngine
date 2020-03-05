#include "ltpch.h"
#include "glShader.h"

#include <glad/glad.h>

namespace Light {

	glShader::glShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_ShaderID = glCreateProgram();

		const char* lvs_source = vertexSource.c_str();
		const char* lfs_source = fragmentSource.c_str();

		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &lvs_source, nullptr);
		glCompileShader(vs);

		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &lfs_source, nullptr);
		glCompileShader(fs);

		glAttachShader(m_ShaderID, vs);
		glAttachShader(m_ShaderID, fs);
		glLinkProgram(m_ShaderID);

		glDeleteShader(vs);
		glDeleteShader(fs);

		int linkStatus;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &linkStatus);
		LT_CORE_ASSERT(linkStatus, "glShader::glShader: Failed to link glShader:\nvertexSource:\n{}\nfragmentSource:\n{}",
		               vertexSource, fragmentSource);


		BindTextures(fragmentSource);
	}

	glShader::~glShader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void glShader::Bind()
	{
		glUseProgram(m_ShaderID);
	}

	void glShader::BindTextures(const std::string& fragmentSource)
	{
		int index = 0;
		std::stringstream stream(fragmentSource);
		std::string line;
		Bind();

		while (std::getline(stream, line))
		{
			if (line.find("sampler2D") != std::string::npos)
			{
				line = line.substr(line.find("sampler2D"));
				if (line.find('[') != std::string::npos)
				{
					unsigned int count = std::stoi(line.substr(line.find('[') + 1, line.find(']') - line.find('[')));
					int test = line.find("u_") - line.find(";") - 1;
					std::string name = line.substr(line.find("u_"), line.find(";") - line.find("u_"));
					for (int i = 0; i < count; i++)
					{
						int loc = glGetUniformLocation(m_ShaderID, (name + '[' + std::to_string(i) + ']').c_str());
						LT_CORE_ASSERT(index < 15, "glShader::BindTextures: Too many sampler2D uniforms");
						glUniform1iv(loc, 1, &index);
						index++;
					}
				}
				else
				{
					std::string name = line.substr(line.find("u_"), line.find("u_") - line.find(";"));
					LT_CORE_ASSERT(index < 15, "glShader::BindTextures: Too many sampler2D uniforms");
					glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), index++);
				}
			}
		}
	}

}