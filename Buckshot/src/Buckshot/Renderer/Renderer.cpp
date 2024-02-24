#include <bspch.h>

#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {

  void Renderer::BeginScene()
  {

  }

  void Renderer::EndScene()
  {

  }

  void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
  {
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
  }

}