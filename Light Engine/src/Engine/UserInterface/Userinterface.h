#pragma once

#include "Core/Core.h"

namespace Light {

	class UserInterface
	{
	private:
		static std::unique_ptr<UserInterface> s_Context;
	public:
		virtual ~UserInterface() = default;

		static inline UserInterface* Get() { return s_Context.get(); }

		virtual void Begin() = 0;
		virtual void End  () = 0;

		void ShowImGuiDemoWnidow();
	private:
		friend class GraphicsContext;

		static void Init();
		static void Terminate();
	};

}