#pragma once

#include "Core/Core.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	class dxUserInterface : public UserInterface
	{
	public:
		dxUserInterface();
		~dxUserInterface();

		void Begin() override;
		void End() override;
	};

}