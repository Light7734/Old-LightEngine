#pragma once

#include "Core/Window.h"

#include "Core/Core.h"

#define LT_WINDOWCLASSNAME L"LightEngine Window"

namespace Light {

	class WindowsWindow : public Window 
	{
	private:
		HINSTANCE m_ModuleHandle = nullptr;
		HWND      m_WindowHandle = nullptr;

		DWORD m_Style;
	public:
		WindowsWindow           (const WindowData& data)         ;
		WindowsWindow           (const WindowsWindow&  ) = delete;
		WindowsWindow& operator=(const WindowsWindow&  ) = delete;
		~WindowsWindow          (                      )         ;

		void HandleEvents() override;

		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func) override;

		void Resize    (uint16_t width, uint16_t height) override;
		void Reposition(int16_t  x    , int16_t  y     ) override;

		void SetTitle(const std::string& title) override;

		void SetWindowState(WindowState state) override;
		void SetDisplayMode(DisplayMode mode ) override;

		void SetVSync (bool vSync) override;

		void Center() override;
		void Show  () override;
		void Hide  () override;

		void Close() override;

		inline void* GetHandle() const override { return m_WindowHandle; }
	private:
		void UpdateShowState();

		static LRESULT CALLBACK StaticProcedure          (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK StaticProcedureNoCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK StaticProcedureSetup     (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

}