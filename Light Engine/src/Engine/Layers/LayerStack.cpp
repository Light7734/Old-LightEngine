#include "ltpch.h"
#include "LayerStack.h"

#include <imgui.h>

namespace Light {

	LayerStack* LayerStack::s_Instance = nullptr;

	std::vector<Layer*> LayerStack::s_Layers = {};

	std::vector<Layer*>::iterator LayerStack::s_Current;

	std::vector<Layer*>::iterator LayerStack::s_Begin;

	std::vector<Layer*>::iterator LayerStack::s_End;

	LayerStack::LayerStack()
	{
		LT_CORE_ASSERT(!s_Instance, "LayerStack::LayerStack: LayerStack should not be constructed on client projects");
		s_Instance = this;
	}

	LayerStack::~LayerStack()
	{
		LT_PROFILE_FUNC();

		for (Layer* layer : s_Layers)
		{
			layer->OnDetatch();
			delete layer;
		}
	}

	void LayerStack::AttachLayer(Layer* layer)
	{
		LT_PROFILE_FUNC();

		bool empty = s_Layers.empty();

		if (empty || std::find(s_Layers.begin(), s_Layers.end(), layer) == s_Layers.end())
		{
			Layer* current = nullptr;

			if (!empty && s_Current != s_End)
				current = *s_Current;

			s_Layers.push_back(layer);

			s_Begin = s_Layers.begin();
			s_End = s_Layers.end();

			if (current)
				s_Current = std::find(s_Begin, s_End, current);
			else
				s_Current = s_End;

			layer->OnAttach();
		}
		else
			LT_CORE_ERROR("LayerStack::AttachLayer: layer already exists: {}", layer->GetName());
	}

	void LayerStack::DetachLayer(Layer* layer)
	{
		LT_PROFILE_FUNC();

		if(s_Layers.empty())
			{ LT_CORE_ERROR("LayerStack::DetachLayer: LayerStack is empty"); return; }

		auto it = std::find(s_Layers.begin(),s_Layers.end(), layer);

		if (it != s_Layers.end())
		{
			Layer* current = nullptr;

			if (s_Current != s_End)
				current = *s_Current;

			if (current != layer)
			{
				s_Layers.erase(it);

				s_Begin = s_Layers.begin();
				s_End = s_Layers.end();

				if(current)
					s_Current = std::find(s_Begin, s_End, current);
				else
					s_Current = s_End;
			}
			else
				s_Current = s_Layers.erase(it);

			layer->OnDetatch();
		}
		else
			LT_CORE_ERROR("LayerStack::DetachLayer: failed to find the layer: {}", layer->GetName());
	}

	void LayerStack::ShowDebugWindow()
	{
 		for (int i = 0; i < s_Layers.size(); i++)
 		{
 			if(ImGui::TreeNode(s_Layers[i], "%s [ %s ] (%d)",
 			                                s_Layers[i]->GetName().c_str(),
 			                                s_Layers[i]->IsEnabled() ? "Enabled" : "Disabled",
 			                                i))
 			{
 				s_Layers[i]->ShowDebugWindow();
 				ImGui::TreePop();
 			}
 		}
	}

}