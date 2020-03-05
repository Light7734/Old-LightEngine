#pragma once

#include "Core/Core.h"

namespace Light {

	class Blender
	{
	private:
		static std::unique_ptr<Blender> s_Context;
	protected:
		static bool b_Enabled;
	public:
		static inline void Enable () { b_Enabled = true ; s_Context->EnableImpl();  }
		static inline void Disable() { b_Enabled = false; s_Context->DisableImpl(); }

		static void ShowDebugWindow();

		static inline bool IsEnabled() { return b_Enabled; }

		virtual void EnableImpl() = 0;
		virtual void DisableImpl() = 0;
	private:
		friend class GraphicsContext;
		static void Init();
	};

}