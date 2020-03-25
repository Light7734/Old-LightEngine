#pragma once

#include "Application.h"
#include "Core.h"

#include "Debug/Exceptions.h"

int main()
{
// hide the command line if we are in distribution build
#ifdef LIGHT_DIST
	#ifdef LIGHT_PLATFORM_WINDOWS
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	#endif

	#ifdef LIGHT_PLATFORM_LINUX
	#endif

	#ifdef LIGHT_PLATFORM_MAC
	#endif
#endif

	Light::Application* app = nullptr;
	int exitCode = 0;

	try
	{
		app = Light::CreateApplication();
		LT_CORE_ASSERT(app, "main: Light::Application is not initialized");

		app->GameLoop();
	}
	catch (Light::FailedAssertion fa)
	{
		LT_CORE_FATAL("Application exited due FailedAssertion");
		exitCode = -1;
	}
	catch (Light::glException gle) 
	{
		LT_CORE_FATAL("Application exited due glException");
		exitCode = -2;
	}
	catch (Light::dxException dxe)
	{
		LT_CORE_FATAL("Application exited due dxException");
		exitCode = -3;
	}

	delete app;
	return exitCode;
}