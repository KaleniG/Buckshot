#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Buckshot/Core/Core.h"

namespace Buckshot {

  class Log
  {
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;

  };

}

// Buckshot logging macros
#define BS_INFO(...)  ::Buckshot::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BS_TRACE(...) ::Buckshot::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BS_WARN(...)  ::Buckshot::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BS_ERROR(...) ::Buckshot::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BS_FATAL(...) ::Buckshot::Log::GetCoreLogger()->critical(__VA_ARGS__)

// ClientApp logging macros
#define CL_INFO(...)  ::Buckshot::Log::GetClientLogger()->info(__VA_ARGS__)
#define CL_TRACE(...) ::Buckshot::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CL_WARN(...)  ::Buckshot::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CL_ERROR(...) ::Buckshot::Log::GetClientLogger()->error(__VA_ARGS__)
#define CL_FATAL(...) ::Buckshot::Log::GetClientLogger()->critical(__VA_ARGS__)