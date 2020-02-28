#pragma once

#include "Core/Core.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	class glUserInterface : public UserInterface
	{
	private:
		friend class UserInterface;
		glUserInterface();

		void TerminateImpl() override;


		void BeginImpl() override;
		void EndImpl() override;
	};

}