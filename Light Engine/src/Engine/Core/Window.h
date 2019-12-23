#pragma once

#include "Core/Core.h"

namespace Light {

	class Event;

	enum class WindowStyle
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

		WindowStyle style = WindowStyle::Windowed;
		WindowState state = WindowState::Visible;

		uint16_t width = 0, height = 0;
		int16_t  x     = 0, y      = 0;

		bool vSync      = true;
		bool active     = true; // input focus
		bool running    = true;
		
		std::function<void(Event&)> eventCallback;
	};
	
	class Window
	{
	protected:
		WindowData m_Data;
	public:
		virtual ~Window() = default;

		static std::unique_ptr<Window> Create(const WindowData& data);

		virtual void HandleEvents() = 0;

		virtual void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func) = 0;

		virtual void Resize    (uint16_t width, uint16_t height) = 0;
		virtual void Reposition(int16_t  x    , int16_t  y     ) = 0;

		virtual void SetTitle(const std::string& title) = 0;

		virtual void SetWindowState (WindowState state) = 0;
		virtual void SetWindowStyle (WindowStyle style) = 0;

		virtual void SetVSync (bool vSync) = 0;

		virtual void Center() = 0;
		virtual void Show  () = 0;
		virtual void Hide  () = 0;

		virtual void Close() = 0;


		virtual inline void* GetHandle() const = 0;

		virtual inline std::pair<uint16_t, uint16_t> GetSize    () const { return { m_Data.width, m_Data.height }; }
		virtual inline std::pair<int16_t , int16_t > GetPosition() const { return { m_Data.x    , m_Data.y      }; }

		virtual inline std::string_view GetTitle() const { return m_Data.title; }

		virtual inline WindowState GetWindowState() const { return m_Data.state; }
		virtual inline WindowStyle GetWindowStyle() const { return m_Data.style; }

		virtual inline bool isVSync () const { return m_Data.vSync; }

		virtual inline bool isActive () const { return m_Data.active;  }
		virtual inline bool isRunning() const { return m_Data.running; }
	};

}