#pragma once

// 3rd-party libraries
#include <irrKlang.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

#include <imgui.h>

// Engine
#include "Debug/Logger.h"
#include "Debug/Benchmark/Instrumentor.h"

// Platform
#ifdef _WIN32
	#define NOMINMAX
	#include <Windows.h>
	#undef NOMINMAX
#endif

// Containers
#include <array>
#include <vector>
#include <list>
#include <set>
#include <bitset>
#include <map>
#include <unordered_map>
#include <unordered_set>

// Miscellaneous
#include <algorithm>
#include <functional>
#include <memory>
#include <tuple>
#include <utility>

// IO
#include <iostream>
#include <fstream>
#include <sstream>

// Multi-threading
#include <thread>
#include <atomic>

// String
#include <string>
#include <string_view>

// C-Libraries
#include <time.h>
#include <math.h>