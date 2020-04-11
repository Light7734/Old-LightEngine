#pragma once

#include "Core/Core.h"

struct GLFWmonitor;

namespace Light {

	enum class AspectRatio
	{
		AR_4_3,
		AR_16_9,
		AR_16_10,
	};

	enum class GraphicsAPI
	{
		Default, Opengl, Directx, // #todo: metal
	};

	struct Resolution
	{
		unsigned int width, height;
		float aspectRatio;

		Resolution(unsigned int _width, unsigned int _height, AspectRatio ratio)
			: width(_width), height(_height), aspectRatio((float)_width / _height)
		{
			LT_CORE_ASSERT((aspectRatio == 4.0f / 3.0f) == (ratio == AspectRatio::AR_4_3),
				"Resolution::Resolution: invalid aspect ratio: [{}x{}] != 4:3", width, height);

			LT_CORE_ASSERT((aspectRatio == 16.0f / 9.0f) == (ratio == AspectRatio::AR_16_9),
				"Resolution::Resolution: invalid aspect ratio: [{}x{}] != 16:9", width, height);

			LT_CORE_ASSERT((aspectRatio == 16.0f / 10.0f) == (ratio == AspectRatio::AR_16_10),
				"Resolution::Resolution: invalid aspect ratio: [{}x{}] != 16:10", width, height);
		}

		Resolution() : width(0), height(0), aspectRatio(0.0f) {}


		// operators
		operator bool() const
		{
			return width && height && (float)width / height == aspectRatio;
		}
	};

	struct GraphicsConfigurations
	{
		Resolution resolution;

		unsigned int MSAASampleCount = 4;

		bool MSAAEnabled = false;
		bool vSync = true;
	};

	class GraphicsContext
	{
	private:
		static std::unique_ptr<GraphicsContext> s_Context;
		static GraphicsAPI s_Api;
	protected:
		GraphicsConfigurations m_Configurations;
	public:
		virtual ~GraphicsContext() = default;

		static void CreateContext(GraphicsAPI api, const GraphicsConfigurations& configurations);

		static inline GraphicsContext* Get() { return s_Context.get(); }

		void ShowDebugWindow();

		virtual void SwapBuffers() = 0;

		virtual void ClearBackbuffer(float colors[4]) = 0;

		virtual void Draw(unsigned int count) = 0;
		virtual void DrawIndexed(unsigned int count) = 0;

		virtual void DefaultRenderBuffer() = 0;

		// setters
		virtual void SetConfigurations(const GraphicsConfigurations& configurations) = 0;

		virtual void SetResolution(const Resolution& resolution) = 0;

		virtual void SetVSync(bool enabled) = 0;

		void SetMSAA(bool enabled);
		void SetMSAASampleCount(unsigned int sampleCount);
		
		// getters
		static inline GraphicsConfigurations GetConfigurations() { return s_Context->m_Configurations; }
		static inline GraphicsAPI GetAPI() { return s_Api; }

		static inline Resolution GetResolution() { return s_Context->m_Configurations.resolution; }
		static inline float GetAspectRatio() { return s_Context->m_Configurations.resolution.aspectRatio; }

		static inline unsigned int GetMSAASampleCount() { return s_Context->m_Configurations.MSAASampleCount; }
		static inline bool IsMSAAEnabled() { return s_Context->m_Configurations.MSAAEnabled; }

		static inline bool IsVSync() { return s_Context->m_Configurations.vSync; }
	};

}