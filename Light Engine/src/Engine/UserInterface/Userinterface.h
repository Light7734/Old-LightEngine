#pragma once

#include "Core/Core.h"

namespace Light {

	class UserInterface
	{
	private:
		static std::unique_ptr<UserInterface> s_Context;
	public:
		static inline void Begin() { s_Context->BeginImpl(); };
		static inline void End  () { s_Context->EndImpl();   };

		static void ShowImGuiDemoWnidow();

		virtual void BeginImpl() = 0;
		virtual void EndImpl() = 0;
	private:
		friend class GraphicsContext;
		static void Init();
	};

}