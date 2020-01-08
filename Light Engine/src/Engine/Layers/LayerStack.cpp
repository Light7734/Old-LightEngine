#include "ltpch.h"
#include "LayerStack.h"

namespace Light {

	std::vector<std::shared_ptr<Layer>> LayerStack::s_Layers;

	LayerStack::~LayerStack()
	{
		for (auto layer : s_Layers)
			layer->OnDetatch();
	}

	void LayerStack::AttachLayer(std::shared_ptr<Layer> layer)
	{
		s_Layers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::DetatchLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(s_Layers.begin(), s_Layers.end(), layer);

		if (it != s_Layers.end())
			{ layer->OnDetatch(); s_Layers.erase(it); }
		else
			LT_CORE_ERROR("Failed to find the specified layer from LayerStack");
	}

}