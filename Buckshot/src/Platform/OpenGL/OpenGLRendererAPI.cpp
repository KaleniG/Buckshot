#include <bspch.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Buckshot {

  void OpenGLMessageCallback(
    unsigned source,
    unsigned type,
    unsigned id,
    unsigned severity,
    int length,
    const char* message,
    const void* userParam)
  {
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         BS_FATAL(message); return;
    case GL_DEBUG_SEVERITY_MEDIUM:       BS_ERROR(message); return;
    case GL_DEBUG_SEVERITY_LOW:          BS_WARN(message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: BS_TRACE(message); return;
    }

    BS_ASSERT(false, "Unknown severity level!");
  }

  void OpenGLRendererAPI::Init()
  {
    BS_PROFILE_FUNCTION();

    #ifdef BS_DEBUG
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(OpenGLMessageCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    #endif

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
  }

  void OpenGLRendererAPI::ClearColor(const glm::vec4& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRendererAPI::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRendererAPI::SetLineWidth(float width)
  {
    glLineWidth(width);
  }

  void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t index_count)
  {
    vertexArray->Bind();
    uint32_t count = (index_count) ? index_count : vertexArray->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  }

  void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertex_count)
  {
    vertexArray->Bind();
    glDrawArrays(GL_LINES, 0, vertex_count);
  }

  void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  {
    glViewport(x, y, width, height);
  }

}