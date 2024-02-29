#pragma once

// Reducing the warnings...
#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <memory>
#include <chrono>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Buckshot/Core/Log.h"

#ifdef BS_PLATFORM_WINDOWS
  #include <windows.h>
#endif