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
		if (Buckshot::Input::IsKeyPressed(BS_KEY_TAB))
			BS_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender()
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