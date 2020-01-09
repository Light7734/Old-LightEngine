#include "ltpch.h"
#include "LayerStack.h"

namespace Light {

	std::vector<Layer*> LayerStack::s_Layers;
	std::vector<Layer*> LayerStack::s_PushedLayers;
	std::vector<Layer*> LayerStack::s_PoppedLayers;

	bool LayerStack::b_LayersQueued = false;

	LayerStack::~LayerStack()
	{
		for (Layer* layer : s_Layers)
		{
			layer->OnDetatch();
			delete layer;
		}
	}

	void LayerStack::AttachLayer(Layer* layer)
	{
		b_LayersQueued = true;

		s_PushedLayers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::DetatchLayer(Layer* layer)
	{
		b_LayersQueued = true;

		s_PoppedLayers.push_back(layer);
		layer->OnDetatch();
	}

	void LayerStack::HandleQueuedLayers()
	{
		if (b_LayersQueued)
		{
			b_LayersQueued = false;

			if (s_PushedLayers.size())
			{
				s_Layers.insert(s_Layers.end(), s_PushedLayers.begin(), s_PushedLayers.end());
				s_PushedLayers.clear();
			}

			if (s_PoppedLayers.size())
			{
				for (Layer* layer : s_PoppedLayers)
				{
					auto it = std::find(s_Layers.begin(), s_Layers.end(), layer);

					if (it != s_Layers.end())
						{ s_Layers.erase(it); }
					else
						LT_CORE_ERROR("Failed to find the specified layer from LayerStack");
				}

				s_PoppedLayers.clear();
			}
		}
	}

}