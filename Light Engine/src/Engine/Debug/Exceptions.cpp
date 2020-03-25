#include "ltpch.h"
#include "Exceptions.h"

#include "Platform/Opengl/glDebug/glToString.h"

#include <glad/glad.h>

#ifdef LIGHT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

namespace Light {

	glException::glException(unsigned int source, unsigned int type, unsigned int id, const char* msg)

	{
		// #todo: log more information
		LT_CORE_FATAL("glException has been thrown:");
		LT_CORE_WARN("glMessageCallback: :: Source: {} :: Type: {} :: ID: {}",
		             glToString::DebugMsgSource  (source  ),
		             glToString::DebugMsgType    (type    ), id);
		LT_CORE_FATAL("        Vendor  : {}", glGetString(GL_VENDOR));
		LT_CORE_FATAL("        Renderer: {}", glGetString(GL_RENDERER));
		LT_CORE_FATAL("        Version : {}", glGetString(GL_VERSION));
		LT_CORE_FATAL("        SVersion: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
		LT_CORE_FATAL("        {}", msg);
	}

	LT_DX(
	dxException::dxException(HRESULT hr, const char* file, int line)
	{
		char* message;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		               nullptr, hr,
		               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		               (LPSTR)(&message), NULL, nullptr);

		// #todo: log more information		
		LT_CORE_FATAL("dxException has been thrown:");
		LT_CORE_FATAL("        File: {}, Line: {}", file, line);
		LT_CORE_FATAL("        {}", message);

		LocalFree(message);
	})

}