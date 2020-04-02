#pragma once

#include "Core.h"

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;
struct GLFWcursor;

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
		std::string title = "LightEngineWindow";

		DisplayMode displayMode = DisplayMode::Windowed;

		bool visible   = true;
		bool minimized = false;
	private:
		friend class Window;

		bool active    = true;
		bool closed    = false;

		GLFWcursor* cursor = nullptr;
		
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
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void HandleEvents();

		// graphics
		static inline void GfxSetApi(GraphicsAPI api, const GraphicsConfigurations& configurations)
		{ 
			std::unique_ptr<GraphicsContext> context =  GraphicsContext::Create(api, configurations);

			if(context)
				s_Context->m_GraphicsContext = std::move(context); 
		}

		static inline void GfxSetSetConfigurations(const GraphicsConfigurations& configurations)
		                   { LT_PROFILE_FUNC(); s_Context->m_GraphicsContext->SetConfigurations(configurations); }

		static inline void GfxSetResolution       (const Resolution& resolution)
		                   { LT_PROFILE_FUNC(); s_Context->m_GraphicsContext->SetResolution(resolution); }

		static inline void GfxSetVSync            (bool vsync)
		                   { LT_PROFILE_FUNC(); s_Context->m_GraphicsContext->SetVSync(vsync); }

		// setters
		void SetEventCallbackFunction(std::function<void(Event&)> event_callback_func);

		static void SetMouseCursor(const std::string& texturePath, unsigned int hotspotX = 0, unsigned int hotspotY = 0);

		static void SetTitle(const std::string& title);

		static void SetDisplayMode(DisplayMode mode);

		static void SetVisibility(bool visible);
		static void Center();
		static void Close();

		// getters
		static inline GLFWwindow* GetGlfwHandle  () { return s_Context->m_GlfwHandle;   }
		static inline void*       GetNativeHandle() { return s_Context->m_NativeHandle; }

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