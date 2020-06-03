#pragma once

// Engine
#include "Core/Core.h"

// Platform
#ifdef _WIN32
	#define NOMINMAX
	#include <Windows.h>
	#include <d3d11.h>
	#include <wrl.h>
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