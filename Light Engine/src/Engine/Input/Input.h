#pragma once

#include "Keycodes.h"

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
		static int16_t s_MouseOffX, s_MouseOffY;
		static int16_t s_MousePosX, s_MousePosY;

		static int8_t s_MouseWheelOff;

		static bool s_Keys   [LIGHT_MAX_KEYS   ];
		static bool s_Buttons[LIGHT_MAX_BUTTONS];
	public:
		Input() = delete;

		static void OnEvent(Event& event);

		static bool GetKey   (uint16_t key   ) { return s_Keys   [key   ]; }
		static bool GetButton(uint8_t  button) { return s_Buttons[button]; }


		static inline int16_t GetMouesX() { return s_MousePosX; }
		static inline int16_t GetMouseY() { return s_MousePosY; }

		static inline std::pair<int16_t, int16_t>GetMousePos() { return { s_MousePosX, s_MousePosY }; }


		static inline int16_t GetMouseOffX() { return s_MouseOffX; }
		static inline int16_t GetMouseOffY() { return s_MouseOffY; }

		static inline std::pair<int16_t, int16_t> GetMouseOff() { return { s_MouseOffX, s_MouseOffY }; }


		static inline int8_t GetWheelOff() { return s_MouseWheelOff; }
	private:
		static bool OnKeyPress     (KeyboardKeyPressedEvent&  event);
		static bool OnKeyRelease   (KeyboardKeyReleasedEvent& event);
		static bool OnButtonPress  (MouseButtonPressedEvent&  event);
		static bool OnButtonRelease(MouseButtonReleasedEvent& event);
		static bool OnMouseMove    (MouseMovedEvent&          event);
		static bool OnMouseScrolled(MouseScrolledEvent&       event);
	};

}