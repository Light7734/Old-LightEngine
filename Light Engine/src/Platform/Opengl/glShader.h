#pragma once

#include "Renderer/Shader.h"

#include "Core/Core.h"

namespace Light {

	class glShader : public Shader
	{
	private:
		unsigned int m_ShaderID;
	public:
		glShader(const std::string& vertex_source, const std::string& fragment_source);
		~glShader();

		void Bind() override;
	private:
		void BindTextures(const std::string& fragment_source);
	};

}