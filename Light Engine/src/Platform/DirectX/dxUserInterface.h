#pragma once

#include "UserInterface/Userinterface.h"

#include "Core/Core.h"

namespace Light {

	class dxUserInterface : public UserInterface
	{
	private:
		friend class UserInterface;
		dxUserInterface();

		void BeginImpl() override;
		void EndImpl() override;
		void TerminateImpl() override;
	};

}