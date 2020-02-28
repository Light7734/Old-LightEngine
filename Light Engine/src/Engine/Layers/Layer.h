#pragma once

#include "Core/Core.h"

#include <imgui.h>

namespace Light {

	class Event;
	
	class Layer
	{
	private:
		bool b_Enabled = true;
	protected:
		std::string m_LayerName = "DefaultLayerName";
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

		virtual void ShowDebugWindow() { ImGui::Text("ShowDebugWindow function is not overridden!"); }

		virtual void OnEvent(Event& event) {}


		// Getters
		inline const std::string& GetName() const { return m_LayerName; }
		inline bool IsEnabled() const { return b_Enabled; }
	};

}