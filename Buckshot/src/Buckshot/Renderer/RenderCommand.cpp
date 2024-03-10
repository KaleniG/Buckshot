#include <bspch.h>

#include "Buckshot/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Buckshot {

  Scope<RendererAPI> RenderCommand::m_RendererAPI = RendererAPI::Create();
}