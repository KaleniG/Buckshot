#include <Buckshot.h>

class ExampleLayer : public Buckshot::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnEvent(Buckshot::Event& event) override
	{
	}

};

class Sandbox : public Buckshot::Application
{
public:
  Sandbox()
  {
		PushLayer(new ExampleLayer());
		PushOverlay(new Buckshot::ImGuiLayer());
  }

  ~Sandbox()
  {

  }
};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}