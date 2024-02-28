#pragma once

#include <Buckshot.h>

class Sandbox2D : public Buckshot::Layer
{
public:
  Sandbox2D() : Layer("Sandbox2D") {}
  ~Sandbox2D() override = default;

  void OnAttach() override;
  void OnDetach() override;
  
  void OnUpdate(Buckshot::Timestep timestep) override;
  void OnImGuiRender() override;
  void OnEvent(Buckshot::Event& event) override;

private:
  Buckshot::Ref<Buckshot::Shader> m_Shader;
  Buckshot::Ref<Buckshot::VertexArray> m_VertexArray;
  Buckshot::OrthographicCameraController m_CameraController;

  glm::vec4 m_SquareColor;
};