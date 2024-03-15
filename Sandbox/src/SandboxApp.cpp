#include <Buckshot.h>
#include <Buckshot/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Buckshot::Application
{
public:
  Sandbox()
    : Application("SandboxApp")
  {
		PushLayer(new Sandbox2D());
  }
};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}