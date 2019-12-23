#include "ltpch.h"
#include "WindowsWindow.h"

#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/WindowEvents.h"

#include "Input/Keycodes.h"

#include <Windowsx.h>

namespace Light {

	std::unique_ptr<Window> Window::Create(const WindowData& data)
	{
		return std::make_unique<WindowsWindow>(data);
	}

	WindowsWindow::WindowsWindow(const WindowData& data)
	{
		m_Data = data;
		m_ModuleHandle = GetModuleHandle(NULL); // --


		// Create and register window class
		WNDCLASS wc = { 0 };

		wc.hInstance = m_ModuleHandle;

		wc.lpfnWndProc = WindowsWindow::StaticProcedureSetup;
		wc.lpszClassName = LT_WINDOWCLASSNAME;
		wc.style = CS_OWNDC;

		wc.hbrBackground = nullptr;
		wc.hCursor = nullptr;
		wc.hIcon = nullptr;

		wc.cbWndExtra = 0;
		wc.cbClsExtra = 0;

		RegisterClass(&wc);


		// Adjust window's rect and style
		RECT rect = { 0, 0, data.width, data.height };
		AdjustWindowRect(&rect, m_Style, false);
		SetWindowStyle(data.style);

		// Create window
		m_WindowHandle = CreateWindow( LT_WINDOWCLASSNAME,
		                               std::wstring(data.title.begin(), data.title.end()).c_str(),
		                               m_Style,
		                               data.x, data.y,
		                               rect.right- rect.left, rect.bottom - rect.top,
		                               nullptr,
		                               nullptr,
		                               m_ModuleHandle,
		                               &m_Data );
		// Show window
		UpdateShowState();
	}

	WindowsWindow::~WindowsWindow()
	{
		CloseWindow(m_WindowHandle);
		UnregisterClass(LT_WINDOWCLASSNAME, m_ModuleHandle);
	}

	void WindowsWindow::HandleEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
	}

	void WindowsWindow::SetEventCallbackFunction(std::function<void(Event&)> event_callback_func)
	{
		m_Data.eventCallback = event_callback_func;
	}

	void WindowsWindow::Resize(uint16_t width, uint16_t height)
	{
		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, m_Style, false);

		SetWindowPos( m_WindowHandle, HWND_TOPMOST,
		              NULL, NULL, // position
		              rect.right - rect.left, // width
		              rect.bottom - rect.top, // height
		              SWP_NOMOVE );

		m_Data.width = width;
		m_Data.height = height;
	}

	void WindowsWindow::Reposition(int16_t x, int16_t y)
	{
		SetWindowPos(m_WindowHandle, HWND_TOPMOST, x, y, NULL, NULL, SWP_NOSIZE);

		m_Data.x = x;
		m_Data.y = y;
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		std::wstring stemp = std::wstring(title.begin(), title.end());
		SetWindowText(m_WindowHandle, stemp.c_str());
	}

	void WindowsWindow::SetWindowState(WindowState state)
	{
		m_Data.state = state;
		UpdateShowState();
	}

	void WindowsWindow::SetWindowStyle(WindowStyle style)
	{
		m_Style = style == WindowStyle::Windowed ? WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX : WS_POPUP;

		m_Data.style = style;
		m_Data.state = WindowState::Visible;

		SetWindowLongPtr(m_WindowHandle, GWL_STYLE, m_Style);
		UpdateShowState();
	}

	void WindowsWindow::SetVSync(bool vSync)
	{
		m_Data.vSync = vSync;
	}

	void WindowsWindow::Center()
	{
		RECT rc;
		GetWindowRect(m_WindowHandle, &rc);

		int xPos = (GetSystemMetrics(SM_CXSCREEN) - (rc.right  - rc.left)) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top )) / 2;

		Reposition(xPos, yPos);
	}

	void WindowsWindow::Show()
	{
		m_Data.state = WindowState::Visible;
		UpdateShowState();
	}

	void WindowsWindow::Hide()
	{
		m_Data.state = WindowState::Hidden;
		UpdateShowState();
	}

	void WindowsWindow::Close()
	{
		m_Data.running = false;
	}

	void WindowsWindow::UpdateShowState()
	{
		ShowWindow( m_WindowHandle, m_Data.state == WindowState::Hidden    ? SW_HIDE     :
		                            m_Data.state == WindowState::Minimized ? SW_MINIMIZE :
		                            m_Data.state == WindowState::Visible   ?
		                                m_Data.style == WindowStyle::ExclusiveFullscreen ? SW_MAXIMIZE : SW_RESTORE : SW_RESTORE);
	}

	LRESULT CALLBACK WindowsWindow::StaticProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto* data = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

		switch (msg)
		{
			// KeyboardKey events ====================================================================================
			case WM_KEYDOWN:
			{
				KeyboardKeyPressedEvent event(wParam);
				data->eventCallback(event);
				return 0;
			}
			case WM_KEYUP:
			{
				KeyboardKeyReleasedEvent event(wParam);
				data->eventCallback(event);
				return 0;
			}

			// MouseButton events ====================================================================================
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent event(BUTTON_LEFT);
				data->eventCallback(event);
				return 0;
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event(BUTTON_LEFT);
				data->eventCallback(event);
				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent event(BUTTON_RIGHT);
				data->eventCallback(event);
				return 0;
			}
			case WM_RBUTTONUP: 
			{
				MouseButtonReleasedEvent event(BUTTON_RIGHT);
				data->eventCallback(event);
				return 0;
			}

			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent event(BUTTON_MIDDLE);
				data->eventCallback(event);
				return 0;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event(BUTTON_MIDDLE);
				data->eventCallback(event);
				return 0;
			}

			case WM_XBUTTONDOWN:
			{
				if (HIWORD(wParam) == 1)
				{
					MouseButtonPressedEvent event(BUTTON_X1);
					data->eventCallback(event);
					return 0;
				}
				else
				{
					MouseButtonPressedEvent event(BUTTON_X2);
					data->eventCallback(event);
					return 0;
				}
			}
			case WM_XBUTTONUP:
			{

				if (HIWORD(wParam) == 1)
				{
					MouseButtonReleasedEvent event(BUTTON_X1);
					data->eventCallback(event);
					return 0;
				}
				else
				{
					MouseButtonReleasedEvent event(BUTTON_X2);
					data->eventCallback(event);
					return 0;
				}
			}
			// =======================================================================================================

			// Mouse events ==========================================================================================
			case WM_MOUSEMOVE:
			{
				MouseMovedEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				data->eventCallback(event);
				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				MouseScrolledEvent event(int8_t(HIWORD(wParam)) / int8_t(120));
				data->eventCallback(event);
				return 0;
			}
			// =======================================================================================================


			// Window events =========================================================================================
			case WM_SIZE:
			{
				if (wParam == SIZE_RESTORED)
				{
					WindowRestoredEvent event;
					WindowResizedEvent event2(LOWORD(lParam), HIWORD(lParam));

					data->width = LOWORD(lParam);
					data->height = HIWORD(lParam);

					data->state = WindowState::Visible;
					data->eventCallback(event);
					data->eventCallback(event2);

					return 0;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					WindowMaximizedEvent event;
					WindowResizedEvent event2(LOWORD(lParam), HIWORD(lParam));

					data->width  = LOWORD(lParam);
					data->height = HIWORD(lParam);

					data->state = WindowState::Visible;
					data->eventCallback(event);
					data->eventCallback(event2);

					return 0;
				}
				else if (wParam == SIZE_MINIMIZED)
				{
					WindowMinimizedEvent event;

					data->state = WindowState::Minimized;
					data->eventCallback(event);

					return 0;
				}
			}

			case WM_MOVE:
			{
				WindowMovedEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				data->x = GET_X_LPARAM(lParam);
				data->y = GET_Y_LPARAM(lParam);
				data->eventCallback(event);

				return 0;
			}

			case WM_SETFOCUS:
			{
				WindowFocusedEvent event;

				data->active = true;
				data->eventCallback(event);

				return 0;
			}
			case WM_KILLFOCUS:
			{
				WindowLostFocusEvent event;

				data->active = false;
				data->eventCallback(event);

				return 0;
			}

			case WM_CLOSE:
			{
				WindowClosedEvent event;
				data->eventCallback(event);
				return 0;
			}
			// =======================================================================================================
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::StaticProcedureNoCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (((WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->eventCallback)
		{
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&WindowsWindow::StaticProcedure);
			return WindowsWindow::StaticProcedure(hWnd, msg, wParam, lParam);
		}
		
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::StaticProcedureSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CREATE)
		{
			WindowData* data = (WindowData*) ( (CREATESTRUCT*)lParam )->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC , (LONG_PTR)&WindowsWindow::StaticProcedureNoCallback);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}