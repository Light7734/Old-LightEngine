#pragma once

#include "Core/Core.h"

#include "UserInterface/UserInterface.h"

namespace Light {

	class glUserInterface : public UserInterface
	{
	public:
		glUserInterface();
		~glUserInterface();

		void BeginImpl() override;
		void EndImpl() override;
	};

}