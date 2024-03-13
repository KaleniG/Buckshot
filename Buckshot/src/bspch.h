#pragma once

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
#include "Buckshot/Debug/Instrumentor.h"

#ifdef BS_PLATFORM_WINDOWS
  #include <windows.h>
#endif