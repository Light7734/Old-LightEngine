#include "MainLayer.h"

MainLayer::MainLayer()
{
	LT_CORE_TRACE("MainLayer::MainLayer");
}

MainLayer::~MainLayer()
{
	LT_CORE_TRACE("MainLayer::~MainLayer");
}

void MainLayer::OnUpdate(float DeltaTime)
{
	if (Light::Input::GetKey(KEY_ESCAPE))
		Light::Window::Get()->Close();
}
