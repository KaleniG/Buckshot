#include <Buckshot.h>
#include <Buckshot/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Buckshot {

  class BuckshotEditor : public Application
  {
  public:
    BuckshotEditor()
      : Application("BuckshotEditor")
    {
      PushLayer(new EditorLayer());
    }
  };

  Application* CreateApplication()
  {
    return new BuckshotEditor;
  }

}
