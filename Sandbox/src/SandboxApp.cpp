#include <Buckshot.h>

class Sandbox : public Buckshot::Application
{
public:
  Sandbox()
  {

  }

  ~Sandbox()
  {

  }
};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}