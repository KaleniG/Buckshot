#include <Buckshot.h>
#include <Buckshot/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Buckshot::Application
{
public:
  Sandbox(const Buckshot::ApplicationSpecification& spec)
    : Application(spec)
  {
    PushLayer(new Sandbox2D());
  }
};

Buckshot::Application* Buckshot::CreateApplication(Buckshot::ApplicationCommandLineArgs args)
{
  Buckshot::ApplicationSpecification spec;
  spec.Name = "Buckshot Editor";
  spec.CommandLineArgs = args;

  return new Sandbox(spec);
}