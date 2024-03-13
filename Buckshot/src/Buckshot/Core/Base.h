#pragma once

#include <memory>

#ifdef BS_PLATFORM_WINDOWS
#else
  #error Buckshot supports only Windows!
#endif

#if defined(BS_DEBUG) || defined(BS_RELEASE)
  #define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
  #define CL_ASSERT(x, ...) { if(!(x)) { CL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
  #define BS_ASSERT(x, ...)
  #define CL_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define BS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Buckshot {

  template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}