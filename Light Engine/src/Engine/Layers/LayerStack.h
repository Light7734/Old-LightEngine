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

		static std::vector<Layer*>::iterator s_Current;

		static std::vector<Layer*>::iterator s_Begin;
		static std::vector<Layer*>::iterator s_End;

		// static std::vector<Layer*>::reverse_iterator s_rBegin;
		// static std::vector<Layer*>::reverse_iterator s_rEnd;
	public:
		LayerStack();
		~LayerStack();

		static void AttachLayer(Layer* layer);
		static void DetachLayer(Layer* layer);

		static void ShowDebugWindow();

		// getters
		static unsigned long GetSize() { return s_Layers.size(); }
		static bool IsEmpty() { return s_Layers.empty(); }

		void next() { ++s_Current; }
		void previous() { --s_Current; }

		const std::vector<Layer*>::iterator& begin() { s_Current = s_Begin; return s_Current; }
		const std::vector<Layer*>::iterator& end() { return s_End; }

		const std::vector<Layer*>::iterator& rbegin() { s_Current = s_End; return s_Current; }
		const std::vector<Layer*>::iterator& rend() { return s_Begin; }
	};

}