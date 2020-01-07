#pragma once

#include "Core/Core.h"

struct GLFWwindow;

namespace Light {

	class Event;

	enum class DisplayMode
	{
		ExclusiveFullscreen = 0 ,
		Windowed                ,
		BorderlessWindowed      ,
	};

	enum class WindowState
	{
		Visible = 0 ,
		Hidden      ,
		Minimized   ,
	};

	struct WindowData
	{
		std::string title = "";

		DisplayMode displayMode = DisplayMode::Windowed;
		WindowState state       = WindowState::Visible ;

		unsigned int width = 0, height = 0;
		int          x     = 0, y      = 0;

		bool active  = true; // input focus
		bool running = true;
		
		std::function<void(Event&)> eventCallback = [](Event&) {};
	};
	
	class Window
	{
	private:
		static Window* s_Instance;

		GLFWwindow* m_Window = nullptr;
		void* m_NativeHandle = nullptr;

		WindowData m_Data = {};
	public:
		Window           (const WindowData& data)         ;
		Window           (const Window&         ) = delete;
		Window& operator=(const Window&         ) = delete;
		~Window          (                      )         ;

		void HandleEvents();

		// Setters
		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func);

		void Resize    (unsigned int width, unsigned int height);
		void Reposition(int          x    , int          y     );

		void SetTitle(const std::string& title);

		void SetState      (WindowState state);
		void SetDisplayMode(DisplayMode mode );

		void Center();

		void Close();

		// Getters
		inline GLFWwindow* GetHandle() const { return m_Window; }
		void* GetNativeHandle() const;

		inline std::pair<uint16_t, uint16_t> GetSize    () const { return { m_Data.width, m_Data.height }; }
		inline std::pair<int16_t , int16_t > GetPosition() const { return { m_Data.x    , m_Data.y      }; }

		inline std::string_view GetTitle() const { return m_Data.title; }

		inline WindowState GetWindowState() const { return m_Data.state;       }
		inline DisplayMode GetDisplayMode() const { return m_Data.displayMode; }

		inline bool isActive () const { return m_Data.active;  }
		inline bool isRunning() const { return m_Data.running; }
	private:
		void SetGlfwCallbacks();
	};

}