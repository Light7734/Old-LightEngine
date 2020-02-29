#pragma once

#include "Core/Core.h"

namespace Light {

	class UserInterface
	{
	private:
		static UserInterface* s_Context;
	private:
		friend class GraphicsContext;
		static void Init();
	protected:
		UserInterface() = default;
	public:
		static inline void Terminate() { s_Context->TerminateImpl(); };

		static inline void Begin    () { s_Context->BeginImpl();     };
		static inline void End      () { s_Context->EndImpl();       };

		static void ShowImGuiDemoWnidow();
	protected:
		virtual void BeginImpl() = 0;
		virtual void EndImpl() = 0;
		virtual void TerminateImpl() = 0;
	};

}