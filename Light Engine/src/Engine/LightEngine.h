#pragma once

#include "Core/Core.h"

// Platform ==================
#ifdef LIGHT_PLATFORM_WINDOWS
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
#include "Debug/InputcodeToString.h"
// ---------------------------

// Events --------------------
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
// ---------------------------

// Input ---------------------
#include "Input/Input.h"
#include "Input/InputCodes.h"
// ---------------------------

// Physics ------------------
// --------------------------

// Renderer -----------------
#include "Renderer/BufferLayout.h"
#include "Renderer/Buffers.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/RenderCommand.h"
#include "Platform/DirectX/dxGraphicsContext.h"
// --------------------------

// Layers --------------------
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
// ---------------------------