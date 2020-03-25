#pragma once

#include "Core/Core.h"

#include <imgui.h>

namespace Light {

	class Event;
	
	class Layer
	{
	protected:
		std::string m_LayeDebugrName = "DefaultLayerName";
		bool b_Enabled = true;
	public:
		Layer() = default;
		virtual ~Layer() = default;

		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;

		virtual void Enable () { b_Enabled = true ; }
		virtual void Disable() { b_Enabled = false; }

		virtual void OnAttach () {}
		virtual void OnDetatch() {}

		virtual void OnRender()                {}
		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnUserInterfaceUpdate() {}

		virtual void ShowDebugWindow() { ImGui::Text("ShowDebugWindow function is not overridden!"); }

		virtual void OnEvent(Event& event) {}

		// getters
		inline const std::string& GetName() const { return m_LayeDebugrName; }
		inline bool IsEnabled() const { return b_Enabled; }

	};

}