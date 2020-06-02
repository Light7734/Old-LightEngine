#include "Demo.h"
#include "DemoLayer.h"

#include <Core/EntryPoint.h> // ** must be included in only one .cpp file //

Demo::Demo()
{
	// LT_PROFILE_FUNC / LT_PROFILE_SCOPE profiles a function / scope, the output files:
	//     Create.json, GameLoop.json, Delete.json will be saved on project's root directory
	//     and can be visually seen via chrome://tracing
	LT_PROFILE_FUNC();
	LT_TRACE("Demo::Demo");

	// initialize window
	Light::WindowData wd;
	wd.title = "Demo";
	wd.displayMode = Light::DisplayMode::Windowed;

	// we pass aspect ratio as enum value so the engine can check if the dimensions are correct.
	Light::GraphicsConfigurations gc;
	gc.resolution = Light::Resolution(1280u, 720u, Light::AspectRatio::AR_16_9); 

	m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Directx);

	// attach the DemoLayer ( layer stack will call delete only on **attached** layers on destructor )
	Light::Application::AttachLayer(new DemoLayer);
}

Demo::~Demo()
{
	LT_TRACE("Demo::~Demo");
}

// ** this function's implementation must be in the same .cpp file where EntryPoint is included	** //
Light::Application* Light::CreateApplication() 
{
	return new Demo;
}