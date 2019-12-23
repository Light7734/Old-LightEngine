#pragma once

#include "Core/Core.h"

// Platform ==================
#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

#ifdef LIGHT_PLATFORM_LINUX
#endif

#ifdef LIGHT_PLATFORM_MAC
#endif
// ===========================

// Core ----------------------
#include "Core/Application.h"
#include "Core/Timer.h"
#include "Core/Window.h"
// ---------------------------

// Debug ---------------------
#include "Debug/Logger.h"
#include "Debug/ExitCodes.h"
#include "Debug/KeycodeToString.h"
// ---------------------------

// Events --------------------
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
// ---------------------------

// Input ---------------------
#include "Input/Input.h"
#include "Input/Keycodes.h"
// ---------------------------

// Physics ------------------
// --------------------------

// Renderer -----------------
#include "Renderer/GraphicsContext.h"
#include "Renderer/RenderCommand.h"
#include "Platform/DirectX/GraphicsContext.h"
// --------------------------

// Layers --------------------
#include "Layers/Layer.h"
// ---------------------------