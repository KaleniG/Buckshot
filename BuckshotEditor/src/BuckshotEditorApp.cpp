#include <Buckshot.h>
#include <Buckshot/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Buckshot {

  class BuckshotEditor : public Application
  {
  public:
    BuckshotEditor(const ApplicationSpecification& spec)
      : Application(spec)
    {
      PushLayer(new EditorLayer());
    }
  };

  Application* CreateApplication(ApplicationCommandLineArgs args)
  {
    ApplicationSpecification spec;
    spec.Name = "Buckshot Editor";
    spec.CommandLineArgs = args;

    return new BuckshotEditor(spec);
  }

}