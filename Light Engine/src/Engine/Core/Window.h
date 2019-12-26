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

		bool active     = true; // input focus
		bool running    = true;
		
		std::function<void(Event&)> eventCallback = [](Event&) {};
	};
	
	class Window
	{
	private:
		GLFWwindow* m_Window = nullptr;
		void* m_NativeHandle = nullptr;

		WindowData m_Data = {};
	public:
		Window(const WindowData& data);
		~Window();

		void HandleEvents();

		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func);

		void Resize    (uint16_t width, uint16_t height);
		void Reposition(int16_t  x    , int16_t  y     );

		void SetTitle(const std::string& title);

		void SetWindowState(WindowState state);
		void SetDisplayMode(DisplayMode mode );

		void Center();
		void Show  ();
		void Hide  ();

		void Close();


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