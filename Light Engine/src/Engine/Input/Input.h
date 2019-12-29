#pragma once

#include "InputCodes.h"

#include "Core/Core.h"

#define LIGHT_MAX_KEYS       348
#define LIGHT_MAX_BUTTONS    8

namespace Light {

	class Event;

	class KeyboardKeyPressedEvent ;
	class KeyboardKeyReleasedEvent;
	class MouseButtonPressedEvent ;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent         ;
	class MouseScrolledEvent      ;

	class Input
	{
	private:
		static int s_MouseOffX, s_MouseOffY;
		static int s_MousePosX, s_MousePosY;

		static int s_MouseWheelOff;

		static bool s_Keys   [LIGHT_MAX_KEYS   ];
		static bool s_Buttons[LIGHT_MAX_BUTTONS];
	public:
		Input() = delete;

		static void OnEvent(Event& event);

		static bool GetKey   (int key   ) { return s_Keys   [key   ]; }
		static bool GetButton(int  button) { return s_Buttons[button]; }


		static inline int GetMouesX() { return s_MousePosX; }
		static inline int GetMouseY() { return s_MousePosY; }

		static inline std::pair<int, int>GetMousePos() { return { s_MousePosX, s_MousePosY }; }


		static inline int GetMouseOffX() { return s_MouseOffX; }
		static inline int GetMouseOffY() { return s_MouseOffY; }

		static inline std::pair<int, int> GetMouseOff() { return { s_MouseOffX, s_MouseOffY }; }


		static inline int GetWheelOff() { return s_MouseWheelOff; }
	private:
		static bool OnKeyPress     (KeyboardKeyPressedEvent&  event);
		static bool OnKeyRelease   (KeyboardKeyReleasedEvent& event);
		static bool OnButtonPress  (MouseButtonPressedEvent&  event);
		static bool OnButtonRelease(MouseButtonReleasedEvent& event);
		static bool OnMouseMove    (MouseMovedEvent&          event);
		static bool OnMouseScrolled(MouseScrolledEvent&       event);
	};

}