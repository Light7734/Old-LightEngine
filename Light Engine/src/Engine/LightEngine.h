#pragma once

#include "Core/Core.h"

// Platform =============================================
#ifdef LIGHT_PLATFORM_WINDOWS
#endif

#ifdef LIGHT_PLATFORM_LINUX
#endif

#ifdef LIGHT_PLATFORM_MAC
#endif
// ======================================================


// 3rd-Party libraries ==================================

// glm -----------------------
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// ---------------------------

// ImGui ---------------------
#include <imgui.h>
// ---------------------------

// ======================================================


// Engine ===============================================

// Audio ---------------------
#include "Audio/AudioEngine.h"
// ---------------------------

// Core ----------------------
#include "Core/Application.h"
#include "Core/Monitor.h"
#include "Core/Timer.h"
#include "Core/Window.h"
// ---------------------------

// Debug ---------------------
#include "Debug/Exceptions.h"
#include "Debug/Logger.h"
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

// Layers --------------------
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
// ---------------------------

// Physics -------------------
#include "Physics/Collision.h"
// ---------------------------

// Renderer -----------------
#include "Renderer/Font/Font.h"
#include "Renderer/Font/FontManager.h"
#include "Renderer/Camera.h"
#include "Renderer/CameraController.h"
#include "Renderer/Blender.h"
#include "Renderer/Buffers.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
// --------------------------

// Utility ------------------
#include "Utility/FileManager.h"
// --------------------------

// ======================================================
