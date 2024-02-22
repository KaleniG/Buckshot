#pragma once

namespace Buckshot {

  class GraphicsContext
  {
  public:
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

  };

}