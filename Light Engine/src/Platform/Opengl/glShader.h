#pragma once

#include "Core/Core.h"

#include "Renderer/Shader.h"

namespace Light {

	class glShader : public Shader
	{
	private:
		unsigned int m_ShaderID;
	public:
		glShader(const std::string& vertexSource, const std::string& fragmentSource);
		~glShader();

		void Bind() override;
	private:
		void AssignSamplersSlots(const std::string& fragmentSource);
		void ExtractVertexElements(const std::string& vertexSource);

		VertexElementType GetVertexElementType(const char* typeName);
	};

}