#include "TestingApplication.h"
#include <Core/EntryPoint.h>

#include "MainLayer.h"

TestingApplication::TestingApplication()
{
	LT_PROFILE_FUNC();
	LT_CORE_TRACE("TestingApplication::TestingApplication");

	// initialize window
	Light::WindowData wd;
	wd.title = "TestingApplication";
	wd.displayMode = Light::DisplayMode::Windowed;

	Light::GraphicsConfigurations gc;
	gc.resolution = Light::Resolution(800.0f, 600.0f, Light::AspectRatio::AR_4_3);

	m_Window = std::make_unique<Light::Window>(wd, gc, Light::GraphicsAPI::Opengl);
	
	// attach main layer
	Light::Application::AttachLayer(new MainLayer());
}

TestingApplication::~TestingApplication()
{
	LT_PROFILE_FUNC();
	LT_CORE_TRACE("TestingApplication::~TestingApplication");
}

Light::Application* Light::CreateApplication()
{
	LT_PROFILE_FUNC();
	return new TestingApplication();
}