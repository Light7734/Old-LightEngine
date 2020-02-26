#pragma once

#include "Core/Core.h"

namespace Light {

	class Event;

	class KeyboardKeyPressedEvent ;
	class KeyboardKeyReleasedEvent;

	class MouseButtonPressedEvent ;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent         ;
	class MouseScrolledEvent      ;

	class UserInterface
	{
	private:
		static UserInterface* s_Context;
	private:
		friend class GraphicsContext;
		static void Init();
	protected:
		UserInterface() = default;
	public:
		static inline void Begin() { s_Context->BeginImpl(); };
		static inline void End() { s_Context->EndImpl(); };
		static inline void Terminate() { s_Context->TerminateImpl(); };

		static void ShowImGuiDemoWnidow();

		static void OnEvent(Event& event);
	protected:
		virtual void BeginImpl() = 0;
		virtual void EndImpl() = 0;
		virtual void TerminateImpl() = 0;
	private:
		static bool OnKeyPress(KeyboardKeyPressedEvent& event);
		static bool OnKeyRelease(KeyboardKeyReleasedEvent& event);

		static bool OnMouseMove(MouseMovedEvent& event);
		static bool OnButtonPress(MouseButtonPressedEvent& event);
		static bool OnButtonRelease(MouseButtonReleasedEvent& event);
		static bool OnMouseScroll(MouseScrolledEvent& event);
		
	};

}