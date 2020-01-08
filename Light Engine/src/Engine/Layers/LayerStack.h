#pragma once

#include "Core/Core.h"

#include "Layer.h"

#include <vector>

namespace Light {

	class LayerStack
	{
	private:
		static std::vector<std::shared_ptr<Layer>> s_Layers;
	private:
		friend class Application; // Only Application can create LayerStack, making it a Singleton
		LayerStack() = default;
		~LayerStack();
	public:
		static void AttachLayer(std::shared_ptr<Layer> layer);
		static void DetatchLayer(std::shared_ptr<Layer> layer);

		static unsigned int GetSize() { return s_Layers.size(); }

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return s_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return s_Layers.end(); }

		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return s_Layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return s_Layers.rend(); }
	};

}