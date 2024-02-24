#include <bspch.h>

#include "Buckshot/Log.h"

namespace Buckshot {

  std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

  void Log::Init()
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt("BUCKSHOT");
    s_CoreLogger->set_level(spdlog::level::trace);
    s_ClientLogger = spdlog::stdout_color_mt("BUSHAPP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }

}