#include "ltpch.h"
#include "Monitor.h"

#include "Window.h"

#include <glfw/glfw3.h>

#include <imgui.h>

namespace Light {

	std::vector<std::shared_ptr<Monitor>> Monitor::s_Handles = {};

	GLFWmonitor** Monitor::s_Monitors = nullptr;
	int Monitor::s_Count = 0;

	int Monitor::s_WindowMonitorIndex = 0;

	// video mode
	VideoMode::VideoMode(const GLFWvidmode* mode)
	{
		LT_PROFILE_FUNC();

		static_assert(sizeof(GLFWvidmode) == sizeof(VideoMode));
		memcpy(this, mode, sizeof(this));
	}

	// monitor
	void Monitor::Init()
	{
		LT_PROFILE_FUNC();

		s_Monitors = glfwGetMonitors(&s_Count);

		LT_CORE_ASSERT(s_Count, "Monitor::Init: no monitors are connected or an error has been occurd");

		for (int i = 0; i < s_Count; i++)
			s_Handles.push_back(std::make_shared<Monitor>(i));

		GLFWmonitor* windowMonitor = glfwGetWindowMonitor(Window::GetGlfwHandle());

		for (int i = 0; i < s_Count; i++) 
		{
			if (s_Monitors[i] == windowMonitor)
			{
				s_WindowMonitorIndex = i;
				break;
			}
		}

		glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) 
		{
			LT_PROFILE_SCOPE("glfwMonitorCallback");

			if (event == GLFW_DISCONNECTED)
			{
				s_Handles.back()->m_Index = 0;
				s_Handles.back()->b_Valid = false;

				s_Handles.pop_back();
				s_Monitors = glfwGetMonitors(&s_Count);
			}

			else
			{
				s_Handles.push_back(std::make_shared<Monitor>(s_Count - 1));
				s_Monitors = glfwGetMonitors(&s_Count);
			}
		});
	}

	Monitor::Monitor(int index)
		: m_Index(index), b_Valid(true)
	{
		LT_PROFILE_FUNC();

		if (index >= s_Count)
			{ LT_CORE_ERROR("Monitor::Monitor: index out of range"); b_Valid = false;}
	}

	void Monitor::OnWindowMove()
	{
		LT_PROFILE_FUNC();

		GLFWmonitor* windowMonitor = glfwGetWindowMonitor(Window::GetGlfwHandle());

		for (int i = 0; i < s_Count; i++)
		{
			if (s_Monitors[i] == windowMonitor)
			{
				s_WindowMonitorIndex = i;
				break;
			}
		}
	}

	void Monitor::ShowDebugWindow()
	{
		if (ImGui::TreeNode(GetGlfwHandle(), "%s (%d)", GetName(), GetIndex())) // monitor handle is guaranteed to be unique
		{
			const glm::ivec2   physical = GetPhysicalSize();
			const glm::vec2    scale = GetContentScale();
			const glm::ivec2   pos = GetVirtualPosition();
			const glm::ivec4   area = GetWorkArea();
			const GLFWvidmode* mode = glfwGetVideoMode(s_Monitors[m_Index]);

			ImGui::BulletText("physical size: [%dmm x %dmm]", physical.x, physical.y);
			ImGui::BulletText("content scale: [%f x %f]", scale.x, scale.y);
			ImGui::BulletText("virtual position: [%d x %d]", pos.x, pos.y);
			ImGui::BulletText("work area: [xpos: %d, ypos: %d, width: %d, height: %d]", area.x, area.y, area.z, area.w);

			if (ImGui::TreeNode(mode, "Video mode")) // i don't know if video modes are unique #todo
			{
				ImGui::BulletText("size: [%d x %d]", mode->width, mode->height);
				ImGui::BulletText("bits: [r: %d, g: %d, b: %d]", mode->redBits, mode->greenBits, mode->blueBits);
				ImGui::BulletText("refresh rate: [%d]", mode->refreshRate);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}

	void Monitor::ShowDebugWindowAll()
	{
		for (int i = 0; i < GetCount(); i++)
			GetMonitor(i)->ShowDebugWindow();
	}

	void Monitor::SetUserPointer(void* userPointer)
	{
		glfwSetMonitorUserPointer(s_Monitors[m_Index], userPointer);
	}

	std::shared_ptr<Monitor> Monitor::GetMonitor(unsigned int index)
	{
		if (index >= s_Count)
			{ LT_CORE_ERROR("Monitor::GetMonitor: index out of range"); return nullptr; }

		return s_Handles[index];
	}

	void* Monitor::GetUserPointer() const
	{
		return glfwGetMonitorUserPointer(s_Monitors[m_Index]);
	}

	const char* Monitor::GetName() const
	{
		return glfwGetMonitorName(s_Monitors[m_Index]);
	}

	const VideoMode Monitor::GetVideoMode() const
	{
		return VideoMode(glfwGetVideoMode(s_Monitors[m_Index]));
	}

	std::vector<VideoMode> Monitor::GetVideoModes() const
	{
		int count;
		const GLFWvidmode* glfwVideoModes = glfwGetVideoModes(s_Monitors[m_Index], &count);

		std::vector<VideoMode> engineVideoModes;

		for (int i = 0; i < count; i++)
			engineVideoModes.emplace_back(glfwVideoModes + i);

		return engineVideoModes;
	}

	glm::ivec4 Monitor::GetWorkArea() const
	{
		glm::ivec4 area;
		glfwGetMonitorWorkarea(s_Monitors[m_Index], &area.x, &area.y, &area.z, &area.w);
		return area;
	}

	glm::vec2 Monitor::GetContentScale() const
	{
		glm::vec2 scale;
		glfwGetMonitorContentScale(s_Monitors[m_Index], &scale.x, &scale.y);
		return scale;
	}

	glm::ivec2 Monitor::GetPhysicalSize() const
	{
		glm::ivec2 size;
		glfwGetMonitorPhysicalSize(s_Monitors[m_Index], &size.x, &size.y);
		return size;
	}

	glm::ivec2 Monitor::GetVirtualPosition() const
	{
		glm::ivec2 pos;
		glfwGetMonitorPos(s_Monitors[m_Index], &pos.x, &pos.y);
		return pos;
	}

}