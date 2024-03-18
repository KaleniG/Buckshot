#pragma once

#if defined(BS_DEBUG) || defined(BS_RELEASE)
	#ifdef BS_PLATFORM_WINDOWS
		#define BS_DEBUGBREAK() __debugbreak()
	#elif defined(BS_PLATFORM_LINUX)
		#error Linux is not supported!
		#include <signal.h>
		#define BS_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error Platform doesn't support debugbreak!
	#endif
#endif

#if defined(BS_DEBUG) || defined(BS_RELEASE)
  #define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR("Assertion Failed: {0}", __VA_ARGS__); BS_DEBUGBREAK(); } }
  #define CL_ASSERT(x, ...) { if(!(x)) { CL_ERROR("Assertion Failed: {0}", __VA_ARGS__); BS_DEBUGBREAK(); } }
#else
  #define BS_ASSERT(x, ...)
  #define CL_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define BS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


#include <memory>
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