#pragma once

#include "InputCodes.h"

#include "Core/Core.h"

#include <glm/glm.hpp>

#define LIGHT_MAX_KEYS       348
#define LIGHT_MAX_BUTTONS    8

namespace Light {

	class Camera;

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
		static glm::ivec2 s_MousePos;
		static glm::ivec2 s_MouseOff;

		static int s_MouseWheelOff;

		static bool s_Keys   [LIGHT_MAX_KEYS   ];
		static bool s_Buttons[LIGHT_MAX_BUTTONS];

		static int s_LastKey;
		static int s_LastButton;
	public:
		Input() = delete;

		static void OnEvent(Event& event);

		static glm::vec2 MousePosToCameraView(const std::shared_ptr<Camera> camera);

		static void ShowDebugWindow();

		// getters
		static bool GetKey   (int  key   ) { return s_Keys   [key   ]; }
		static bool GetButton(int  button) { return s_Buttons[button]; }

		static inline int GetMouseX() { return s_MousePos.x; }
		static inline int GetMouseY() { return s_MousePos.y; }

		static inline const glm::ivec2& GetMousePos() { return s_MousePos; }

		static inline int GetMouseOffX() { return s_MouseOff.x; }
		static inline int GetMouseOffY() { return s_MouseOff.y; }

		static inline const glm::ivec2& GetMouseOff() { s_MouseOff; }

		static inline int GetWheelOff() { return s_MouseWheelOff; }

		static std::string GetKeyName(int key);
		static std::string GetButtonName(int button);
	private:
		static bool OnKeyPress     (KeyboardKeyPressedEvent&  event);
		static bool OnKeyRelease   (KeyboardKeyReleasedEvent& event);

		static bool OnButtonPress  (MouseButtonPressedEvent&  event);
		static bool OnButtonRelease(MouseButtonReleasedEvent& event);
		static bool OnMouseMove    (MouseMovedEvent&          event);
		static bool OnMouseScrolled(MouseScrolledEvent&       event);
	};

}