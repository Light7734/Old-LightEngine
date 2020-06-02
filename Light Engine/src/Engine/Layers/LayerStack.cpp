#include "ltpch.h"
#include "LayerStack.h"

#include <imgui.h>

namespace Light {

	LayerStack::~LayerStack()
	{
		LT_PROFILE_FUNC();

		for (Layer* layer : m_Layers)
		{
			layer->OnDetatch();
			delete layer;
		}
	}

	void LayerStack::AttachLayer(Layer* layer)
	{
		LT_PROFILE_FUNC();

		bool empty = m_Layers.empty();

		if (empty || std::find(m_Layers.begin(), m_Layers.end(), layer) == m_Layers.end())
		{
			Layer* current = nullptr;

			if (!empty && m_Current != m_End)
				current = *m_Current;

			m_Layers.push_back(layer);

			m_Begin = m_Layers.begin();
			m_End = m_Layers.end();

			if (current)
				m_Current = std::find(m_Begin, m_End, current);
			else
				m_Current = m_End;
		}
		else
			LT_CORE_ERROR("LayerStack::AttachLayer: layer already exists: {}", layer->GetName());

		m_Layers.back()->SetDrawProiority((m_Layers.size() - 1));
	}

	void LayerStack::DetachLayer(Layer* layer)
	{
		LT_PROFILE_FUNC();

		if(m_Layers.empty())
			{ LT_CORE_ERROR("LayerStack::DetachLayer: LayerStack is empty"); return; }

		auto it = std::find(m_Layers.begin(),m_Layers.end(), layer);

		if (it != m_Layers.end())
		{
			Layer* current = nullptr;

			if (m_Current != m_End)
				current = *m_Current;

			if (current != layer)
			{
				m_Layers.erase(it);

				m_Begin = m_Layers.begin();
				m_End = m_Layers.end();

				if(current)
					m_Current = std::find(m_Begin, m_End, current);
				else
					m_Current = m_End;
			}
			else
				m_Current = m_Layers.erase(it);
		}
		else
			LT_CORE_ERROR("LayerStack::DetachLayer: failed to find the layer: {}", layer->GetName());

		for (int i = 0; i < m_Layers.size(); i++)
			m_Layers[i]->SetDrawProiority(i);
	}

	void LayerStack::ShowDebugWindow()
	{
 		for (int i = 0; i < m_Layers.size(); i++)
 		{
 			if(ImGui::TreeNode(m_Layers[i], "%s [ %s ] (%d)",
 			                                m_Layers[i]->GetName().c_str(),
 			                                m_Layers[i]->IsEnabled() ? "Enabled" : "Disabled",
 			                                i))
 			{
 				m_Layers[i]->ShowDebugWindow();
 				ImGui::TreePop();
 			}
 		}
	}

}