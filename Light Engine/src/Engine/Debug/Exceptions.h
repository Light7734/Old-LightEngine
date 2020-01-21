#pragma once

#include "Core/Core.h"

#define DXC(x) hr = x; if(FAILED(x)) { LT_DBREAK; throw dxException(hr, __FILE__, __LINE__); }

namespace Light {

	struct glException : std::exception
	{
		glException(unsigned int source, unsigned int type, unsigned int id, const char* msg);
	};

	LT_DX(
	struct dxException : public std::exception
	{
		dxException(HRESULT hr, const char* file, int line);
	};)
}