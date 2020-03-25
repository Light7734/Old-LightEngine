#include "ltpch.h"
#include "glShader.h"

#include <glad/glad.h>

namespace Light {

	glShader::glShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// create program
		m_ShaderID = glCreateProgram();

		// store c_str() in local variables so then we can use '&' operator on them (address of operator requires an lvalue)
		const char* lvs_source = vertexSource.c_str();
		const char* lfs_source = fragmentSource.c_str();

		// create shaders
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &lvs_source, nullptr);
		glCompileShader(vs);

		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &lfs_source, nullptr);
		glCompileShader(fs);

		// attach and link shaders to the program
		glAttachShader(m_ShaderID, vs);
		glAttachShader(m_ShaderID, fs);
		glLinkProgram(m_ShaderID);

		// delete the shaders
		glDeleteShader(vs);
		glDeleteShader(fs);

		
		// verify link status
		int linkStatus;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &linkStatus);
		LT_CORE_ASSERT(linkStatus, "glShader::glShader: failed to link glShader:\nvertexSource:\n{}\nfragmentSource:\n{}",
		               vertexSource, fragmentSource);


		// bind sampler2D(s) and sampler2D[](s) to the right slots
		AssignSamplersSlots(fragmentSource);
	}

	glShader::~glShader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void glShader::Bind()
	{
		glUseProgram(m_ShaderID);
	}

	void glShader::AssignSamplersSlots(const std::string& fragmentSource)
	{
		int index = 0;
		std::stringstream stream(fragmentSource);
		std::string line;
		Bind();

		while (std::getline(stream, line))
		{
			if (line.find("sampler2D") != std::string::npos)
			{
				LT_CORE_ASSERT(index < 15, "glShader::BindTextures: too many sampler2D uniforms");

				line = line.substr(line.find("sampler2D"));
		
				// #todo: this is so tricky and requires consistency when we declare a uniform sampler... improve:
				if (line.find('[') != std::string::npos)
				{
					unsigned int count = std::stoi(line.substr(line.find('[') + 1, line.find(']') - line.find('[')));
					std::string name = line.substr(line.find("u_"), line.find(";") - line.find("u_"));

					for (int i = 0; i < count; i++)
					{
						glUniform1iv(glGetUniformLocation(m_ShaderID, (name + '[' + std::to_string(i) + ']').c_str()), 1, &index);
						index++;
					}
				}
				else
				{
					std::string name = line.substr(line.find("u_"), (line.find(";") - line.find("u_") ));
					glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), index++);
				}
			}
		}
	}

}