#pragma once

#include <LightEngine.h>

class MainLayer : public Light::Layer 
{
private:
public:
	MainLayer();
	~MainLayer();

	void OnUpdate(float DeltaTime) override;
};