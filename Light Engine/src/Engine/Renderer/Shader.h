#pragma once

#include "Core/Core.h"

namespace Light {

	class Shader
	{
	protected:
		int m_InitStatus;
	public:
		virtual ~Shader() = default;

		static std::shared_ptr<Shader> Create(const std::string& vertex_path, const std::string& fragment_path);

		virtual void Bind() = 0;

		virtual int InitStatus() const { return m_InitStatus; }
	private:
		static void ExtractShaderSource(std::string& src, const std::string& delim);
	};

}