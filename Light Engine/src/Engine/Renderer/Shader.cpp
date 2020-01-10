#include "ltpch.h"
#include "Shader.h"

#include "GraphicsContext.h"

#include "Platform/DirectX/dxShader.h"
#include "Platform/Opengl/glShader.h"

#include "Utility/FileManager.h"

namespace Light {

	std::shared_ptr<Light::Shader> Shader::Create(const std::string& vertex, const std::string& fragment)
	{
		bool vsSourceInFile = vertex.find('\n') == std::string::npos;
		bool fsSourceInFile = fragment.find('\n') == std::string::npos;

		std::string vertex_source = vsSourceInFile ? FileManager::ReadTxtFile(vertex) : vertex;
		std::string fragment_source = fsSourceInFile ? FileManager::ReadTxtFile(fragment) : fragment;

		LT_CORE_ASSERT(!vertex_source.empty(), EC_FILE_READ_FAIL_SHADER, "Empty vertex shader source");
		LT_CORE_ASSERT(!fragment_source.empty(), EC_FILE_READ_FAIL_SHADER, "Empty fragment shader source");


		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Default:
			LT_CORE_ASSERT(false, EC_NO_INIT_GRAPHICSC_CONTEXT, "Failed to create Shader: GraphicsContext::Init was never called");

		case GraphicsAPI::DirectX:
			ExtractShaderSource(vertex_source, "HLSL");
			ExtractShaderSource(fragment_source, "HLSL");

			return std::make_shared<dxShader>(vertex_source, fragment_source);

		case GraphicsAPI::Opengl:
			ExtractShaderSource(vertex_source, "GLSL");
			ExtractShaderSource(fragment_source, "GLSL");

			return std::make_shared<glShader>(vertex_source, fragment_source);

		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}
	}

	void Shader::ExtractShaderSource(std::string& src, const std::string& delim)
	{
		unsigned int begDelimPos, endDelimPos;

		begDelimPos = src.find("+" + delim) + 5;
		endDelimPos = src.find("-" + delim);

		LT_CORE_ASSERT(begDelimPos != std::string::npos + 5, EC_ENGINE_ASSERTION, 
		               "Failed to find start delimiter in shader source, delim: {}, shader: \n{}", delim, src);
		LT_CORE_ASSERT(endDelimPos != std::string::npos, EC_ENGINE_ASSERTION,
		               "Failed to find end delimiter in shader source, delim: {}, shader: \n{}", delim, src);

		src = src.substr(begDelimPos, endDelimPos - begDelimPos);
	}

}