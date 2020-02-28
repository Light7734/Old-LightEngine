#pragma once

#include "Core/Core.h"

namespace Light {

	class Shader
	{
	public:
		static std::shared_ptr<Shader> Create(const std::string& vertex, const std::string& fragment);
		virtual ~Shader() = default;


		virtual void Bind() = 0;
	private:
		static void ExtractShaderSource(std::string& src, const std::string& delim);
	};

}