#pragma once

#include "Layer.h"

#include "Core/Core.h"

#include <vector>

namespace Light {

	class LayerStack
	{
	private:
		static LayerStack* s_Instance;

		static std::vector<Layer*> s_Layers;
		static std::vector<Layer*> s_PushedLayers;
		static std::vector<Layer*> s_PoppedLayers;

		static bool b_LayersQueued;
	public:
		LayerStack();
		~LayerStack();

		static void AttachLayer(Layer* layer);
		static void DetatchLayer(Layer* layer);

		static void ShowDebugWindow();

		void HandleQueuedLayers();

		// getters
		static unsigned int GetSize() { return static_cast<unsigned int>(s_Layers.size()); }

		std::vector<Layer*>::iterator begin() { return s_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return s_Layers.end(); }

		std::vector<Layer*>::reverse_iterator rbegin() { return s_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return s_Layers.rend(); }
	};

}