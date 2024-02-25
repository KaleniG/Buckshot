#pragma once

#include <memory>

#ifdef BS_PLATFORM_WINDOWS
#else
  #error Buckshot supports only Windows!
#endif

#ifdef BS_ENABLE_ASSERTS
  #define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
  #define CL_ASSERT(x, ...) { if(!(x)) { CL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
  #define BS_ASSERT(x, ...)
  #define CL_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define BS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Buckshot {

  template <typename T>
  using Scope = std::unique_ptr<T>;

  template <typename T>
  using Ref = std::shared_ptr<T>;
}