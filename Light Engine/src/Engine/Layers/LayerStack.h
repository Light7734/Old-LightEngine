#pragma once

#include "Layer.h"

#include "Core/Core.h"

#include <vector>

namespace Light {

	class LayerStack
	{
	private:
		std::vector<Layer*> s_Layers;

		std::vector<Layer*>::iterator s_Current;

		std::vector<Layer*>::iterator s_Begin;
		std::vector<Layer*>::iterator s_End;
	public:
		~LayerStack();

		void AttachLayer(Layer* layer);
		void DetachLayer(Layer* layer);

		void ShowDebugWindow();

		// getters
		unsigned long GetSize() { return s_Layers.size(); }
		bool IsEmpty() { return s_Layers.empty(); }

		void next() { ++s_Current; }
		void previous() { --s_Current; }

		const std::vector<Layer*>::iterator& begin() { s_Current = s_Begin; return s_Current; }
		const std::vector<Layer*>::iterator& end() { return s_End; }

		const std::vector<Layer*>::iterator& rbegin() { s_Current = s_End; return s_Current; }
		const std::vector<Layer*>::iterator& rend() { return s_Begin; }
	};

}