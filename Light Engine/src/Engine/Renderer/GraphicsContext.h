#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

namespace Light {

	class Window;

	enum class GraphicsAPI
	{
		None = 0, Opengl, DirectX
	};

	class GraphicsContext
	{
	private:
		static GraphicsAPI s_Api;
	protected:
		bool b_VSync = true;
	public:
		GraphicsContext() {}
		virtual ~GraphicsContext() = default;


		static void Init(std::shared_ptr<Window> game_window, bool v_sync);


		static void        SetAPI(GraphicsAPI api) { s_Api = api ; }
		static GraphicsAPI GetAPI(               ) { return s_Api; }

		virtual void SetVSync(bool v_sync)       { b_VSync = v_sync; }
		virtual bool GetVSync(           ) const { return b_VSync;   }


		virtual void SwapBuffers() = 0;

		virtual void Clear() = 0;
		virtual void ClearBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

		virtual void HandleWindowEvents(Event& event) = 0;
	};

}