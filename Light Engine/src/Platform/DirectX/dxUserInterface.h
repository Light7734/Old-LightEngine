#pragma once

#include "Core/Core.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	class dxUserInterface : public UserInterface
	{
	public:
		dxUserInterface();
		~dxUserInterface();

		void BeginImpl() override;
		void EndImpl() override;
	};

}