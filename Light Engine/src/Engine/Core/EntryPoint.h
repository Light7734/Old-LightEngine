#pragma once

#include "Application.h"
#include "Core.h"

int main ()
{
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
		app->GameLoop();
	}
	catch (Light::TerminationReq req)
	{
		LT_CORE_ERROR("Error code: {}", req.exitCode);

		delete app;

		return req.exitCode;
	}

	delete app;
	return EC_SUCCESSFULL_EXECUTION;
}