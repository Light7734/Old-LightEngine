#pragma once

#include <LightEngine.h>

class QuadLayer : public Light::Layer
{
public:
	void OnAttach() override
	{
		LT_TRACE("Attached QuadLayer");
	}

	void OnDetatch() override
	{
		LT_TRACE("Detatched  QuadLayer");
	}

	void OnRender() override
	{
		Light::Renderer::DrawQuad(-0.5f, -0.5f, 1.0f, 1.0f);
	}
};