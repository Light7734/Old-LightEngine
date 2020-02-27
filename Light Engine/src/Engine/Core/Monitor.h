#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

struct GLFWmonitor;
struct GLFWvidmode;

namespace Light {

	// #todo: Add more stuff (gamma ramp, etc)
	class Monitor
	{
	private:
		static GLFWmonitor** s_Monitors;
		static int s_Count;

		static std::vector<std::shared_ptr<Monitor>> s_Handles;
	private:
		int m_Index;
		bool b_Valid;
	private:
		friend class Window;
		static void Init();
	public:
		Monitor(int index);

		// NULL for primary monitor
		static std::shared_ptr<Monitor> GetMonitor(unsigned int index);
		
		static inline unsigned int GetCount() { return s_Count; }

		const char* GetName() const;

		inline int GetIndex() { return m_Index; }
		inline bool IsValid() { return b_Valid; }

		inline GLFWmonitor* GetGlfwHandle() const { return s_Monitors[m_Index]; };

		glm::ivec2 GetPhysicalSize   () const;
		glm::vec2  GetContentScale   () const;
		glm::ivec2 GetVirtualPosition() const;

		glm::ivec4 GetWorkArea() const;

		const GLFWvidmode* GetVideoMode() const;

		void* GetUserPointer() const;
		void SetUserePointer(void* userPointer);

		void ShowDebugWindow();

		static void ShowDebugWindowAll();

		operator bool() const
		{
			return b_Valid && m_Index < s_Count;
		}
	};

}