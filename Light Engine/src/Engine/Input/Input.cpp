#include "ltpch.h"
#include "Input.h"

#include "Events/Event.h"

#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"

namespace Light {

	int16_t Input::s_MousePosX = 0, Input::s_MousePosY = 0;
	int16_t Input::s_MouseOffX = 0, Input::s_MouseOffY = 0;

	int8_t  Input::s_MouseWheelOff = 0;

	bool Input::s_Keys[LIGHT_MAX_KEYS];
	bool Input::s_Buttons[LIGHT_MAX_BUTTONS];

	void Input::OnEvent(Event& event)
	{
		Dispatcher disatcher(event);

		disatcher.Dispatch<KeyboardKeyPressedEvent > (LT_EVENT_FN_STATIC(Input::OnKeyPress     ));
		disatcher.Dispatch<KeyboardKeyReleasedEvent> (LT_EVENT_FN_STATIC(Input::OnKeyRelease   ));

		disatcher.Dispatch<MouseButtonPressedEvent > (LT_EVENT_FN_STATIC(Input::OnButtonPress  ));
		disatcher.Dispatch<MouseButtonReleasedEvent> (LT_EVENT_FN_STATIC(Input::OnButtonRelease));

		disatcher.Dispatch<MouseMovedEvent         > (LT_EVENT_FN_STATIC(Input::OnMouseMove    ));
		disatcher.Dispatch<MouseScrolledEvent      > (LT_EVENT_FN_STATIC(Input::OnMouseScrolled));
	}

	bool Input::OnKeyPress(KeyboardKeyPressedEvent& event)
	{
		s_Keys[event.GetKey()] = true;
		return false;
	}

	bool Input::OnKeyRelease(KeyboardKeyReleasedEvent& event)
	{
		s_Keys[event.GetKey()] = false;
		return false;
	}

	bool Input::OnButtonPress(MouseButtonPressedEvent& event)
	{
		s_Buttons[event.GetButton()] = true;
		return false;
	}

	bool Input::OnButtonRelease(MouseButtonReleasedEvent& event)
	{
		s_Buttons[event.GetButton()] = false;
		return false;
	}

	bool Input::OnMouseMove(MouseMovedEvent& event)
	{
		int16_t prevX = s_MousePosX, prevY = s_MousePosY;

		s_MousePosX = event.GetX();
		s_MousePosY = event.GetY();

		s_MouseOffX = s_MousePosX - prevX;
		s_MouseOffY = s_MousePosY - prevY;

		return false;
	}

	bool Input::OnMouseScrolled(MouseScrolledEvent& event)
	{
		s_MouseWheelOff = event.GetOffset();
		return false;
	}

}