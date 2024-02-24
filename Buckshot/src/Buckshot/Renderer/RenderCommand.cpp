#include <bspch.h>

#include "Buckshot/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Buckshot {

  RendererAPI* RenderCommand::m_RendererAPI = new OpenGLRendererAPI;
}