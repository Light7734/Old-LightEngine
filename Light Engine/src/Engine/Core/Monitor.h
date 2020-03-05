#pragma once

#include "Core.h"

#include <glm/glm.hpp>

struct GLFWmonitor;
struct GLFWvidmode;

namespace Light {

	struct VideoMode
	{
		int width, height;
		int redBits, greenBits, blueBits;
		int refreshRate;

		VideoMode(const GLFWvidmode* mode);

		operator bool() { return width && height; }
	};

	class Monitor
	{
	private:
		static std::vector<std::shared_ptr<Monitor>> s_Handles;
		static GLFWmonitor** s_Monitors;
		static int s_Count;

		static int s_WindowMonitorIndex;

		int m_Index;
		bool b_Valid;
	public:
		Monitor(int index);

		static void OnWindowMove();

		static void ShowDebugWindowAll();

		void ShowDebugWindow();

		// Setters
		void SetUserPointer(void* userPointer);

		// Getters
		static std::shared_ptr<Monitor> GetMonitor(unsigned int index);

		static inline std::shared_ptr<Monitor> GetWindowMonitor() { return s_Handles[s_WindowMonitorIndex]; }

		static inline unsigned int GetCount() { return s_Count; }

		void* GetUserPointer() const;

		const char* GetName() const;

		const VideoMode GetVideoMode() const;

		glm::ivec4 GetWorkArea() const;
		glm::vec2 GetContentScale() const;
		glm::ivec2 GetPhysicalSize() const;
		glm::ivec2 GetVirtualPosition() const;

		inline GLFWmonitor* GetGlfwHandle() const { return s_Monitors[m_Index]; };

		inline int GetIndex() const { return m_Index; }
		inline bool IsValid() const { return b_Valid; }

		// Operators
		operator bool() const
		{
			return b_Valid && m_Index < s_Count;
		}
	private:
		friend class Window;
		static void Init();
	};

}