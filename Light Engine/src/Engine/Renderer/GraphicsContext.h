#pragma once

#include "Core/Core.h"

namespace Light {

	class Event;

	enum class GraphicsAPI
	{
		Default, Opengl, Directx
	};

	struct GraphicsConfigurations
	{
		bool vSync = false;
	};

	class GraphicsContext
	{
	private:
		static GraphicsAPI s_Api;
	protected:
		GraphicsConfigurations m_Configurations;
	public:
		GraphicsContext           (                      ) = default;
		GraphicsContext           (const GraphicsContext&) = delete ;
		GraphicsContext& operator=(const GraphicsContext&) = delete ;
		virtual ~GraphicsContext  (                      ) = default;

		static void CreateContext(GraphicsAPI api, const GraphicsConfigurations& configurations);

		virtual void HandleWindowEvents(Event& event) = 0;

		virtual void EnableVSync () = 0;
		virtual void DisableVSync() = 0;

		virtual void SwapBuffers() = 0;

		virtual void Clear() = 0;
		virtual void ClearBuffer(float r, float g, float b, float a) = 0;

		virtual void Draw(unsigned int count) = 0;
		virtual void DrawIndexed(unsigned int count) = 0;

		// Getters
		static GraphicsAPI GetAPI() { return s_Api; }
	};

}