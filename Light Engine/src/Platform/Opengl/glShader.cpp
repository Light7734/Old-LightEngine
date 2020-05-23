#include "ltpch.h"
#include "glShader.h"

#include <glad/glad.h>

namespace Light {

	glShader::glShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		LT_PROFILE_FUNC();

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
		ExtractVertexElements(vertexSource);
		AssignSamplersSlots(fragmentSource);
	}

	glShader::~glShader()
	{
		LT_PROFILE_FUNC();
		glDeleteProgram(m_ShaderID);
	}

	void glShader::Bind()
	{
		glUseProgram(m_ShaderID);
	}

	void glShader::ExtractVertexElements(const std::string& vertexSource)
	{
		LT_PROFILE_FUNC();

		std::stringstream stream(vertexSource.substr(0, vertexSource.find("void main()")));
		std::string line;

		while (std::getline(stream, line))
		{
			if (line.find("layout(location") != std::string::npos)
			{
				line = line.substr(line.find("in") + 3);
				std::string type = line.substr(0, line.find(' '));
				std::string name = line.substr(line.find(' ') + 1, line.find(';') - line.find(' ') - 1);

				m_Elements.push_back({name, GetVertexElementType(type.c_str())});
			}
		}
	}

	void glShader::AssignSamplersSlots(const std::string& fragmentSource)
	{
		LT_PROFILE_FUNC();

		// #todo: optimize
		std::stringstream stream(fragmentSource.substr(0, fragmentSource.find("void main()")));
		std::string line;
		Bind();

		while (std::getline(stream, line))
		{
			if (line.find("sampler2D") != std::string::npos)
			{
				line = line.substr(line.find("sampler2D"));
		
				std::string name = line.substr(line.find("u_"), (line.find(";") - line.find("u_") ));
				int slot = std::stoi(line.substr(line.find("#BINDING_") + 9, 2));

				glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), slot);
			}
		}
	}

	VertexElementType glShader::GetVertexElementType(const char* typeName)
	{
		switch (hashStr(typeName))
		{
		case hashStr("int"):       return VertexElementType::Int;
		case hashStr("ivec2"):     return VertexElementType::Int2;
		case hashStr("ivec3"):     return VertexElementType::Int3;
		case hashStr("ivec4"):     return VertexElementType::Int4;
							       
		case hashStr("uint"):      return VertexElementType::UInt;
		case hashStr("uvec2"):     return VertexElementType::UInt2;
		case hashStr("uvec3"):     return VertexElementType::UInt3;
		case hashStr("uvec4"):     return VertexElementType::UInt4;
							       
		case hashStr("float"):     return VertexElementType::Float;
		case hashStr("vec2"):      return VertexElementType::Float2;
		case hashStr("vec3"):      return VertexElementType::Float3;
		case hashStr("vec4"):      return VertexElementType::Float4;
							       
		case hashStr("double"):    return VertexElementType::Double;
		case hashStr("dvec2"):     return VertexElementType::Double2;
		case hashStr("dvec3"):     return VertexElementType::Double3;
		case hashStr("dvec4"):     return VertexElementType::Double4;

		default: LT_CORE_ASSERT(false, "glShader::GetElementType: invalid typeName");
		}
	}

}