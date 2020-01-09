#pragma once

#include "Core/Core.h"

namespace Light {

	class Event;
	
	class Layer
	{
	private:
		bool b_Enabled = true;
	public:
		Layer           (            ) = default;
		Layer           (const Layer&) = delete ;
		Layer& operator=(const Layer&) = delete ;
		virtual ~Layer  (            ) = default;

		virtual void Enable () { b_Enabled = true ; }
		virtual void Disable() { b_Enabled = false; }

		virtual void OnAttach () {}
		virtual void OnDetatch() {}

		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnRender(               ) {}
		virtual void OnUserInterfaceUpdate(  ) {}

		virtual void OnEvent(Event& event) {}

		inline bool IsEnable() const { return b_Enabled; }
	};

}