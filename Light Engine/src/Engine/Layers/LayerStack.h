#pragma once

#include "Layer.h"

#include "Core/Core.h"

#include <vector>

namespace Light {

	class LayerStack
	{
	private:
		std::vector<Layer*> m_Layers;

		std::vector<Layer*>::iterator m_Current;

		std::vector<Layer*>::iterator m_Begin;
		std::vector<Layer*>::iterator m_End;
	public:
		~LayerStack();

		void AttachLayer(Layer* layer);
		void DetachLayer(Layer* layer);

		void ShowDebugWindow();

		// getters
		unsigned long GetSize() { return m_Layers.size(); }
		bool IsEmpty() { return m_Layers.empty(); }

		void next() { ++m_Current; }
		void previous() { --m_Current; }

		const std::vector<Layer*>::iterator& begin() { m_Current = m_Begin; return m_Current; }
		const std::vector<Layer*>::iterator& end() { return m_End; }

		const std::vector<Layer*>::iterator& rbegin() { m_Current = m_End; return m_Current; }
		const std::vector<Layer*>::iterator& rend() { return m_Begin; }
	};

}