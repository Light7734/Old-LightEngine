#pragma once

#include "Core/Core.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	class dxUserInterface : public UserInterface
	{
	private:
		friend class UserInterface;
		dxUserInterface();

		void TerminateImpl() override;


		void BeginImpl() override;
		void EndImpl() override;
	};

}