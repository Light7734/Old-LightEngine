#pragma once

#include "Core/Monitor.h"

#include "Core/Core.h"

class GLFWmonitor;

namespace Light {

	enum class AspectRatio
	{
		AR_4_3 = 1,
		AR_16_9,
		AR_16_10,
	};

	struct Resolution
	{
		unsigned int width, height;
		float aspectRatio;

		Resolution(unsigned int _width, unsigned int _height, AspectRatio ratio)
			: width(_width), height(_height), aspectRatio((float)_width / _height)
		{
			LT_CORE_ASSERT((aspectRatio == 4.0f / 3.0f) == (ratio == AspectRatio::AR_4_3),
				"Resolution::Resolution: Invalid aspect ratio: [{}x{}] != 4:3", width, height);

			LT_CORE_ASSERT((aspectRatio == 16.0f / 9.0f) == (ratio == AspectRatio::AR_16_9),
				"Resolution::Resolution: Invalid aspect ratio: [{}x{}] != 16:9", width, height);

			LT_CORE_ASSERT((aspectRatio == 16.0f / 10.0f) == (ratio == AspectRatio::AR_16_10),
				"Resolution::Resolution: Invalid aspect ratio: [{}x{}] != 16:10", width, height);
		}

		Resolution() : width(0), height(0), aspectRatio(0.0f) {}

		operator bool() const
		{
			return width && height && (float)width / height == aspectRatio;
		}
	};


	enum class GraphicsAPI
	{
		Default, Opengl, Directx, // #todo: Metal
	};

	// #todo: improve
	struct GraphicsProperties
	{
		std::shared_ptr<Monitor> primaryMonitor;
		Resolution primaryMonitorRes;
	};

	struct GraphicsConfigurations
	{
		Resolution resolution;
		bool vSync = true;
	};


	class Event;

	class GraphicsContext
	{
	private:
		static GraphicsAPI s_Api;
	protected:
		static GraphicsProperties s_Properties;
		static GraphicsConfigurations s_Configurations;
	protected:
		friend class Window;
		GraphicsContext           (                      ) = default;
		GraphicsContext           (const GraphicsContext&) = delete ;
		GraphicsContext& operator=(const GraphicsContext&) = delete ;

		static std::unique_ptr<GraphicsContext> Create(GraphicsAPI api, const GraphicsConfigurations& configurations);
	public:
		virtual ~GraphicsContext() = default;
		
		// Rendering stuff
		virtual void SwapBuffers() = 0;

		virtual void ClearBackbuffer(float colors[4]) = 0;

		virtual void Draw(unsigned int count) = 0;
		virtual void DrawIndexed(unsigned int count) = 0;

		// Setters
		virtual void SetConfigurations(const GraphicsConfigurations& configurations) = 0;
		virtual void SetResolution    (const Resolution& resolution                ) = 0;
		virtual void SetVSync         (bool vsync                                  ) = 0;

		// Getters
		static inline const GraphicsConfigurations& GetConfigurations() { return s_Configurations; }
		static inline const GraphicsProperties&     GetProperties    () { return s_Properties;     }
		static inline const GraphicsAPI             GetAPI           () { return s_Api;            }
		

		static inline const Resolution& GetResolution() { return s_Configurations.resolution; }
		static inline float GetAspectRatio() { return s_Configurations.resolution.aspectRatio; }

		static inline bool IsVSync() { return s_Configurations.vSync; }
	protected:
	};

}