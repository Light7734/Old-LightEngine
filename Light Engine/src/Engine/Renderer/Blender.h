#pragma once

#include "Core/Core.h"

namespace Light {

	enum BlendMode
	{

	};

	class Blender
	{
	private:
		static Blender* s_Context;
	protected:
		static bool b_Enabled;
	private:
		friend class GraphicsContext;
		static void Init();
	public:
		static inline void Enable() { s_Context->EnableImpl(); }
		static inline void Disable() { s_Context->DisableImpl(); }

		static void ShowDebugWindow();

		static inline bool IsEnabled() { return b_Enabled; }
	protected:
		virtual void EnableImpl() = 0;
		virtual void DisableImpl() = 0;
	};

}