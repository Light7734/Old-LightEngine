#pragma once

#include "Application.h"

#include "Core/Core.h"

int main()
{
// Hide the command line if we are in distribution build
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

	try
	{
		app = Light::CreateApplication();
		LT_CORE_ASSERT(app, "Light::Application is not initialized!");

		app->GameLoop();
	}
	catch (Light::TerminationReq req)
	{
		delete app;
		return -1;
	}

	delete app;
	return 0;
}