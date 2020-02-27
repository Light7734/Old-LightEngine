#include "ltpch.h"
#include "Input.h"

#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

#include "Renderer/GraphicsContext.h"

#include <glfw/glfw3.h>

#include <imgui.h>

namespace Light {

	glm::ivec2 Input::s_MouseOff;
	glm::ivec2 Input::s_MousePos;

	int  Input::s_MouseWheelOff = 0;

	bool Input::s_Keys[LIGHT_MAX_KEYS];
	bool Input::s_Buttons[LIGHT_MAX_BUTTONS];

	int Input::s_LastKey = -1;
	int Input::s_LastButton = -1;

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
		if (event.GetKey() == GLFW_KEY_UNKNOWN)
			return false;

		s_LastKey = event.GetKey();
		s_Keys[event.GetKey()] = true;
		return false;
	}

	bool Input::OnKeyRelease(KeyboardKeyReleasedEvent& event)
	{
		if (event.GetKey() == GLFW_KEY_UNKNOWN)
			return false;

		s_Keys[event.GetKey()] = false;
		return false;
	}

	bool Input::OnButtonPress(MouseButtonPressedEvent& event)
	{
		s_LastButton = event.GetButton();
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
		glm::ivec2 prev = s_MousePos;

		s_MousePos = event.GetPos();
		s_MouseOff = s_MousePos - prev;

		return false;
	}

	bool Input::OnMouseScrolled(MouseScrolledEvent& event)
	{
		s_MouseWheelOff = event.GetOffset();
		return false;
	}

	glm::vec2 Input::MousePosToCameraView(const Camera& camera)
	{
		glm::vec2 mouse = (glm::vec2)s_MousePos;

		mouse.x /= GraphicsContext::GetResolution().width;
		mouse.y /= GraphicsContext::GetResolution().height;

		mouse.x = -camera.GetZoomLevel() + (mouse.x * camera.GetZoomLevel() * 2);
		mouse.x *= camera.GetAspectRatio();

		mouse.y = -camera.GetZoomLevel() + (mouse.y * camera.GetZoomLevel() * 2);

		mouse += camera.GetPosition();

		return mouse;
	}

	std::string Input::GetKeyName(int key)
	{
		static std::unordered_map<int, std::string> keyMap =
		{
			LT_MAP_MACRO_TO_NAME(KEY_SPACE), // 32
			LT_MAP_MACRO_TO_NAME(KEY_APOSTROPHE), // 39
			LT_MAP_MACRO_TO_NAME(KEY_COMMA), // 44
			LT_MAP_MACRO_TO_NAME(KEY_MINUS), // 45
			LT_MAP_MACRO_TO_NAME(KEY_PERIOD), // 46
			LT_MAP_MACRO_TO_NAME(KEY_SLASH), // 47
			LT_MAP_MACRO_TO_NAME(KEY_0), // 48
			LT_MAP_MACRO_TO_NAME(KEY_1), // 49
			LT_MAP_MACRO_TO_NAME(KEY_2), // 50
			LT_MAP_MACRO_TO_NAME(KEY_3), // 51
			LT_MAP_MACRO_TO_NAME(KEY_4), // 52
			LT_MAP_MACRO_TO_NAME(KEY_5), // 53
			LT_MAP_MACRO_TO_NAME(KEY_6), // 54
			LT_MAP_MACRO_TO_NAME(KEY_7), // 55
			LT_MAP_MACRO_TO_NAME(KEY_8), // 56
			LT_MAP_MACRO_TO_NAME(KEY_9), // 57
			LT_MAP_MACRO_TO_NAME(KEY_SEMICOLON), // 59
			LT_MAP_MACRO_TO_NAME(KEY_EQUAL), // 61
			LT_MAP_MACRO_TO_NAME(KEY_A), // 65
			LT_MAP_MACRO_TO_NAME(KEY_B), // 66
			LT_MAP_MACRO_TO_NAME(KEY_C), // 67
			LT_MAP_MACRO_TO_NAME(KEY_D), // 68
			LT_MAP_MACRO_TO_NAME(KEY_E), // 69
			LT_MAP_MACRO_TO_NAME(KEY_F), // 70
			LT_MAP_MACRO_TO_NAME(KEY_G), // 71
			LT_MAP_MACRO_TO_NAME(KEY_H), // 72
			LT_MAP_MACRO_TO_NAME(KEY_I), // 73
			LT_MAP_MACRO_TO_NAME(KEY_J), // 74
			LT_MAP_MACRO_TO_NAME(KEY_K), // 75
			LT_MAP_MACRO_TO_NAME(KEY_L), // 76
			LT_MAP_MACRO_TO_NAME(KEY_M), // 77
			LT_MAP_MACRO_TO_NAME(KEY_N), // 78
			LT_MAP_MACRO_TO_NAME(KEY_O), // 79
			LT_MAP_MACRO_TO_NAME(KEY_P), // 80
			LT_MAP_MACRO_TO_NAME(KEY_Q), // 81
			LT_MAP_MACRO_TO_NAME(KEY_R), // 82
			LT_MAP_MACRO_TO_NAME(KEY_S), // 83
			LT_MAP_MACRO_TO_NAME(KEY_T), // 84
			LT_MAP_MACRO_TO_NAME(KEY_U), // 85
			LT_MAP_MACRO_TO_NAME(KEY_V), // 86
			LT_MAP_MACRO_TO_NAME(KEY_W), // 87
			LT_MAP_MACRO_TO_NAME(KEY_X), // 88
			LT_MAP_MACRO_TO_NAME(KEY_Y), // 89
			LT_MAP_MACRO_TO_NAME(KEY_Z), // 90
			LT_MAP_MACRO_TO_NAME(KEY_LEFT_BRACKET), // 91 
			LT_MAP_MACRO_TO_NAME(KEY_BACKSLASH), // 92 
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT_BRACKET), // 93 
			LT_MAP_MACRO_TO_NAME(KEY_GRAVE_ACCENT), // 96 
			LT_MAP_MACRO_TO_NAME(KEY_WORLD_1), // 161
			LT_MAP_MACRO_TO_NAME(KEY_WORLD_2), // 162
			LT_MAP_MACRO_TO_NAME(KEY_ESCAPE), // 256
			LT_MAP_MACRO_TO_NAME(KEY_ENTER), // 257
			LT_MAP_MACRO_TO_NAME(KEY_TAB), // 258
			LT_MAP_MACRO_TO_NAME(KEY_BACKSPACE), // 259
			LT_MAP_MACRO_TO_NAME(KEY_INSERT), // 260
			LT_MAP_MACRO_TO_NAME(KEY_DELETE), // 261
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT), // 262
			LT_MAP_MACRO_TO_NAME(KEY_LEFT), // 263
			LT_MAP_MACRO_TO_NAME(KEY_DOWN), // 264
			LT_MAP_MACRO_TO_NAME(KEY_UP), // 265
			LT_MAP_MACRO_TO_NAME(KEY_PAGE_UP), // 266
			LT_MAP_MACRO_TO_NAME(KEY_PAGE_DOWN), // 267
			LT_MAP_MACRO_TO_NAME(KEY_HOME), // 268
			LT_MAP_MACRO_TO_NAME(KEY_END), // 269
			LT_MAP_MACRO_TO_NAME(KEY_CAPS_LOCK), // 280
			LT_MAP_MACRO_TO_NAME(KEY_SCROLL_LOCK), // 281
			LT_MAP_MACRO_TO_NAME(KEY_NUM_LOCK), // 282
			LT_MAP_MACRO_TO_NAME(KEY_PRINT_SCREEN), // 283
			LT_MAP_MACRO_TO_NAME(KEY_PAUSE), // 284
			LT_MAP_MACRO_TO_NAME(KEY_F1), // 290
			LT_MAP_MACRO_TO_NAME(KEY_F2), // 291
			LT_MAP_MACRO_TO_NAME(KEY_F3), // 292
			LT_MAP_MACRO_TO_NAME(KEY_F4), // 293
			LT_MAP_MACRO_TO_NAME(KEY_F5), // 294
			LT_MAP_MACRO_TO_NAME(KEY_F6), // 295
			LT_MAP_MACRO_TO_NAME(KEY_F7), // 296
			LT_MAP_MACRO_TO_NAME(KEY_F8), // 297
			LT_MAP_MACRO_TO_NAME(KEY_F9), // 298
			LT_MAP_MACRO_TO_NAME(KEY_F10), // 299
			LT_MAP_MACRO_TO_NAME(KEY_F11), // 300
			LT_MAP_MACRO_TO_NAME(KEY_F12), // 301
			LT_MAP_MACRO_TO_NAME(KEY_F13), // 302
			LT_MAP_MACRO_TO_NAME(KEY_F14), // 303
			LT_MAP_MACRO_TO_NAME(KEY_F15), // 304
			LT_MAP_MACRO_TO_NAME(KEY_F16), // 305
			LT_MAP_MACRO_TO_NAME(KEY_F17), // 306
			LT_MAP_MACRO_TO_NAME(KEY_F18), // 307
			LT_MAP_MACRO_TO_NAME(KEY_F19), // 308
			LT_MAP_MACRO_TO_NAME(KEY_F20), // 309
			LT_MAP_MACRO_TO_NAME(KEY_F21), // 310
			LT_MAP_MACRO_TO_NAME(KEY_F22), // 311
			LT_MAP_MACRO_TO_NAME(KEY_F23), // 312
			LT_MAP_MACRO_TO_NAME(KEY_F24), // 313
			LT_MAP_MACRO_TO_NAME(KEY_F25), // 314
			LT_MAP_MACRO_TO_NAME(KEY_KP_0), // 320
			LT_MAP_MACRO_TO_NAME(KEY_KP_1), // 321
			LT_MAP_MACRO_TO_NAME(KEY_KP_2), // 322
			LT_MAP_MACRO_TO_NAME(KEY_KP_3), // 323
			LT_MAP_MACRO_TO_NAME(KEY_KP_4), // 324
			LT_MAP_MACRO_TO_NAME(KEY_KP_5), // 325
			LT_MAP_MACRO_TO_NAME(KEY_KP_6), // 326
			LT_MAP_MACRO_TO_NAME(KEY_KP_7), // 327
			LT_MAP_MACRO_TO_NAME(KEY_KP_8), // 328
			LT_MAP_MACRO_TO_NAME(KEY_KP_9), // 329
			LT_MAP_MACRO_TO_NAME(KEY_KP_DECIMAL), // 330
			LT_MAP_MACRO_TO_NAME(KEY_KP_DIVIDE), // 331
			LT_MAP_MACRO_TO_NAME(KEY_KP_MULTIPLY), // 332
			LT_MAP_MACRO_TO_NAME(KEY_KP_SUBTRACT), // 333
			LT_MAP_MACRO_TO_NAME(KEY_KP_ADD), // 334
			LT_MAP_MACRO_TO_NAME(KEY_KP_ENTER), // 335
			LT_MAP_MACRO_TO_NAME(KEY_KP_EQUAL), // 336
			LT_MAP_MACRO_TO_NAME(KEY_LEFT_SHIFT), // 340
			LT_MAP_MACRO_TO_NAME(KEY_LEFT_CONTROL), // 341
			LT_MAP_MACRO_TO_NAME(KEY_LEFT_ALT), // 342
			LT_MAP_MACRO_TO_NAME(KEY_LEFT_SUPER), // 343
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT_SHIFT), // 344
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT_CONTROL), // 345
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT_ALT), // 346
			LT_MAP_MACRO_TO_NAME(KEY_RIGHT_SUPER), // 347
			LT_MAP_MACRO_TO_NAME(KEY_MENU), // 348
		};

		return keyMap[key].empty() ? std::to_string(key) : keyMap[key];
	}

	std::string Input::GetButtonName(int button)
	{
		static std::unordered_map<int, std::string> buttonMap =
		{
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_LEFT), // 0
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_RIGHT), // 1
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_MIDDLE), // 2 
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_4), // 3
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_5), // 4
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_6), // 5
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_7), // 6
			LT_MAP_MACRO_TO_NAME(MOUSE_BUTTON_8), // 7
		};
		
		return buttonMap[button].empty() ? std::to_string(button) : buttonMap[button];
	}

	void Input::ShowDebugWindow()
	{
		ImGui::Begin("Light::Input");
		if (ImGui::TreeNode(&s_Keys, "Keyboard"))
		{
			ImGui::Text("last key: %s", GetKeyName(s_LastKey).c_str());

			ImGui::Text("keys down:");
			for (int i = 0; i < LIGHT_MAX_KEYS; i++)
				if (s_Keys[i])
					ImGui::BulletText("%s", GetKeyName(i).c_str());

			ImGui::TreePop();
		}
		if (ImGui::TreeNode(&s_Buttons, "Mouse"))
		{
			ImGui::Text("last button: %s", GetButtonName(s_LastButton).c_str());

			ImGui::Text("buttons down:");
			for (int i = 0; i < LIGHT_MAX_BUTTONS; i++)
				if (s_Buttons[i])
					ImGui::BulletText("%s", GetButtonName(i).c_str());

			ImGui::Text("position: [%d x %d]", s_MousePos.x, s_MousePos.y);
			ImGui::Text("offset (updated per event): [%d x %d]", s_MouseOff.x, s_MouseOff.y);
			ImGui::Text("wheel offset (updated per event): [%d]", s_MouseWheelOff);

			ImGui::TreePop();
		}
		ImGui::End();
	}

}