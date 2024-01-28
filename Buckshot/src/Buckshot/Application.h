#pragma once

#include "Core.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Buckshot {

  class BS_API Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();
  };

  Application* CreateApplication();
}