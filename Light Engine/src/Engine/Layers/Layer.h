#pragma once

#include "Core/Core.h"

namespace Light {

	class Event;
	
	class Layer
	{
	public:
		Layer           (            ) = default;
		Layer           (const Layer&) = delete ;
		Layer& operator=(const Layer&) = delete ;
		virtual ~Layer  (            ) = default;

		virtual void OnAttach () {}
		virtual void OnDetatch() {}

		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnRender(               ) {}
		virtual void OnUserInterfaceUpdate(  ) {}

		virtual void OnEvent(Event& event) {}
	};

}