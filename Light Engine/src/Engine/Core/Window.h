#pragma once

#include "Core/Core.h"

struct GLFWwindow;

namespace Light {

	class Event;

	enum class DisplayMode
	{
		Fullscreen,
		Windowed,
		BorderlessWindowed,
	};

	struct WindowData
	{
		std::string title = "LightEngine Window";

		DisplayMode displayMode = DisplayMode::Windowed;

		unsigned int width = 800, height = 600;
		int          x     = 0, y      = 0;

		bool visible = true;
		bool active  = true; // input focus
		bool closed  = false;
		
		std::function<void(Event&)> eventCallback = [](Event&) {};
	};
	
	class Window
	{
	private:
		static Window* s_Instance;
		static WindowData s_Data;

		static GLFWwindow* s_GlfwHandle;
		static void* s_NativeHandle;
	public:
		Window           (const WindowData& data)         ;
		Window           (const Window&         ) = delete;
		Window& operator=(const Window&         ) = delete;
		~Window          (                      )         ;

		void HandleEvents();

		// Setters
		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func);

		static void Resize    (unsigned int width, unsigned int height);
		static void Reposition(int          x    , int          y     );

		static void SetTitle(const std::string& title);

		static void SetDisplayMode(DisplayMode mode );

		static void SetVisibility(bool visible);

		static void Center();

		static void Close();

		// Getters
		static GLFWwindow* GetGlfwHandle  () { return s_GlfwHandle;   }
		static void*       GetNativeHandle() { return s_NativeHandle; }

		static inline unsigned int GetWidth () { return s_Data.width ; }
		static inline unsigned int GetHeight() { return s_Data.height; }

		static inline int GetPosX() { return s_Data.x; }
		static inline int GetPosY() { return s_Data.y; }

		static inline const std::string& GetTitle() { return s_Data.title; }

		static inline DisplayMode GetDisplayMode() { return s_Data.displayMode; }

		static inline bool IsInitialized() { return s_Instance; }

		static inline bool IsVisible() { return s_Data.visible; }
		static inline bool IsActive () { return s_Data.active;  }
		static inline bool IsClosed () { return s_Data.closed; }
	private:
		void SetGlfwCallbacks();
	};

}