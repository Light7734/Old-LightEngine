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

		bool visible   = true;
		bool minimized = false;
	private:
		friend class Window;
		bool active    = true;
		bool closed    = false;
		
		std::function<void(Event&)> eventCallback = [](Event&) {};
	};
	
	class Window
	{
	private:
		static Window* s_Context;
	private:
		WindowData m_Data;

		GLFWwindow* m_GlfwHandle;
		void* m_NativeHandle;

		std::unique_ptr<GraphicsContext> m_GraphicsContext;
	public:
		Window(const WindowData& data, const GraphicsConfigurations& configurations, GraphicsAPI api);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		void HandleEvents();

		// Graphics
		static void GfxSetApi(GraphicsAPI api, const GraphicsConfigurations& configurations);

		static inline void GfxSetSetConfigurations(const GraphicsConfigurations& configurations)
		                   { s_Context->m_GraphicsContext->SetConfigurations(configurations); }

		static inline void GfxSetResolution       (const Resolution& resolution)
		                   { s_Context->m_GraphicsContext->SetResolution(resolution); }

		static inline void GfxSetVSync            (bool vsync)
		                   { s_Context->m_GraphicsContext->SetVSync(vsync); }

		// Setters
		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func);

		static void SetTitle(const std::string& title);

		static void SetDisplayMode(DisplayMode mode);

		static void SetVisibility(bool visible);

		static void Center();

		static void Close();

		// Getters
		static GLFWwindow* GetGlfwHandle  () { return s_Context->m_GlfwHandle;   }
		static void*       GetNativeHandle() { return s_Context->m_NativeHandle; }

		static inline const std::string& GetTitle() { return s_Context->m_Data.title; }

		static inline DisplayMode GetDisplayMode() { return s_Context->m_Data.displayMode; }

		static inline bool IsInitialized() { return s_Context; }

		static inline bool IsClosed   () { return  s_Context->m_Data.closed   ; }
		static inline bool IsMinimized() { return  s_Context->m_Data.minimized; }
		static inline bool IsVisible  () { return  s_Context->m_Data.visible  ; }
		static inline bool IsActive   () { return  s_Context->m_Data.active   ; }
	private:
		void SetGlfwCallbacks();
	};

}