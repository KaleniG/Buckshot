#include <Buckshot.h>
#include <ImGui/imgui.h>

class ExampleLayer : public Buckshot::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		
	}

	void OnImGuiRender() override
	{
	}

};

class Sandbox : public Buckshot::Application
{
public:
  Sandbox()
  {
		PushLayer(new ExampleLayer());
  }

  ~Sandbox()
  {

  }


};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}