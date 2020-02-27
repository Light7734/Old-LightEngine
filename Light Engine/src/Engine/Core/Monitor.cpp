#include "ltpch.h"
#include "Monitor.h"

#include <glfw/glfw3.h>

#include <imgui.h>

namespace Light {

	GLFWmonitor** Monitor::s_Monitors = nullptr;
	int Monitor::s_Count = 0;
	std::vector<std::shared_ptr<Monitor>> Monitor::s_Handles;

	Monitor::Monitor(int index)
		: m_Index(index), b_Valid(true)
	{
		if (index >= s_Count)
			{ LT_CORE_ERROR("Monitor::Monitor: index out of range"); b_Valid = false;}
	}

	std::shared_ptr<Monitor> Monitor::GetMonitor(unsigned int index)
	{
		if (index >= s_Count)
			{ LT_CORE_ERROR("Monitor::GetMonitor: index out of range"); return nullptr; }

		return s_Handles[index];
	}

	void Monitor::Init()
	{
		s_Monitors = glfwGetMonitors(&s_Count);
		LT_CORE_ASSERT(s_Count, "Monitor::Init: No monitors are connected or an error has been occurd");

		for (int i = 0; i < s_Count; i++)
			s_Handles.push_back(std::make_shared<Monitor>(i));

		glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) 
		{
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

	const char* Monitor::GetName() const
	{
		return glfwGetMonitorName(s_Monitors[m_Index]);
	}

	glm::ivec2 Monitor::GetPhysicalSize() const
	{
		glm::ivec2 size;
		glfwGetMonitorPhysicalSize(s_Monitors[m_Index], &size.x, &size.y);
		return size;
	}

	glm::vec2 Monitor::GetContentScale() const
	{
		glm::vec2 scale;
		glfwGetMonitorContentScale(s_Monitors[m_Index], &scale.x, &scale.y);
		return scale;
	}

	glm::ivec2 Monitor::GetVirtualPosition() const
	{
		glm::ivec2 pos;
		glfwGetMonitorPos(s_Monitors[m_Index], &pos.x, &pos.y);
		return pos;
	}

	glm::ivec4 Monitor::GetWorkArea() const
	{
		glm::ivec4 area;
		glfwGetMonitorWorkarea(s_Monitors[m_Index], &area.x, &area.y, &area.z, &area.w);
		return area;
	}

	const GLFWvidmode* Monitor::GetVideoMode() const
	{
		return glfwGetVideoMode(s_Monitors[m_Index]);
	}

	void Monitor::SetUserePointer(void* userPointer)
	{
		glfwSetMonitorUserPointer(s_Monitors[m_Index], userPointer);
	}


	void* Monitor::GetUserPointer() const
	{
		return glfwGetMonitorUserPointer(s_Monitors[m_Index]);
	}

	void Monitor::ShowDebugWindow()
	{
		ImGui::Begin("Light::Monitor");
		if (ImGui::TreeNode(GetGlfwHandle(), "%s (%d):", GetName(), GetIndex())) // monitor handle is guaranteed to be unique
		{
			const glm::ivec2   physical = GetPhysicalSize();
			const glm::vec2    scale    = GetContentScale();
			const glm::ivec2   pos      = GetVirtualPosition();
			const glm::ivec4   area     = GetWorkArea();
			const GLFWvidmode* mode     = GetVideoMode();


			ImGui::BulletText("physical size: [%dmm x %dmm]", physical.x, physical.y);
			ImGui::BulletText("content scale: [%f x %f]", scale.x, scale.y);
			ImGui::BulletText("virtual position: [%d x %d]", pos.x, pos.y);
			ImGui::BulletText("work area: [xpos: %d, ypos: %d, width: %d, height: %d]", area.x, area.y, area.z, area.w);

			if (ImGui::TreeNode(mode, "Video mode:")) // I don't know if video modes are unique #todo
			{
				ImGui::BulletText("size: [%d x %d]", mode->width, mode->height);
				ImGui::BulletText("bits: [r: %d, g: %d, b: %d]", mode->redBits, mode->greenBits, mode->blueBits);
				ImGui::BulletText("refresh rate: [%d]", mode->refreshRate);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void Monitor::ShowDebugWindowAll()
	{
		for (int i = 0; i < GetCount(); i++)
			GetMonitor(i)->ShowDebugWindow();
	}
}