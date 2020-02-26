#pragma once

#include "UserInterface/Userinterface.h"

#include "Core/Core.h"

namespace Light {

	class glUserInterface : public UserInterface
	{
	private:
		friend class UserInterface;
		glUserInterface();

		void BeginImpl() override;
		void EndImpl() override;
		void TerminateImpl() override;
	};

}