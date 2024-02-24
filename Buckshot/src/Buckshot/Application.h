#pragma once

#include "Buckshot/Core.h"
#include "Buckshot/Window.h"
#include "Buckshot/LayerStack.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/ImGui/ImGuiLayer.h"
#include "Buckshot/Renderer/Shader.h"
#include "Buckshot/Renderer/Buffers.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  class Application
  {
  public:
    Application();
    virtual ~Application();
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

  private:
    bool OnWindowClose(WindowCloseEvent& e);

  private:
    static Application* s_Instance;
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    bool m_Running = true;

    // TEMP
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexArray> m_SquareVertexArray;

    OrthographicCamera m_Camera;
  };

  Application* CreateApplication();
}