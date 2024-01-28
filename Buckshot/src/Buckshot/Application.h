#pragma once

#include "Core.h"

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