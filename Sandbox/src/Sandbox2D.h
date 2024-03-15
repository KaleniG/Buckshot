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
  Buckshot::OrthographicCameraController m_CameraController;

  Buckshot::Ref<Buckshot::Texture2D> m_Texture;
  Buckshot::Ref<Buckshot::Texture2D> m_SpriteSheet;
  Buckshot::Ref<Buckshot::SubTexture2D> m_BarrelTexture;
};