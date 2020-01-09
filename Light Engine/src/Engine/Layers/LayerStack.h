#pragma once

#include "Core/Core.h"

#include "Layer.h"

#include <vector>

namespace Light {

	class LayerStack
	{
	private:
		static std::vector<Layer*> s_Layers;
		static std::vector<Layer*> s_PushedLayers;
		static std::vector<Layer*> s_PoppedLayers;
		static bool b_LayersQueued;
	private:
		friend class Application; // Only Application can create LayerStack, making it a Singleton
		LayerStack() = default;
		~LayerStack();
	public:
		static void AttachLayer(Layer* layer);
		static void DetatchLayer(Layer* layer);

		static unsigned int GetSize() { return s_Layers.size(); }

		std::vector<Layer*>::iterator begin() { return s_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return s_Layers.end(); }

		std::vector<Layer*>::reverse_iterator rbegin() { return s_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return s_Layers.rend(); }
	private:
		void HandleQueuedLayers();
	};

}