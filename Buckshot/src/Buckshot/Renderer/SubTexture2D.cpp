#include <bspch.h>

#include "Buckshot/Renderer/SubTexture2D.h"

namespace Buckshot {

  SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
  {
    m_Texture = texture;
    m_TexCoords[0] = glm::vec2(min.x, min.y);
    m_TexCoords[1] = glm::vec2(max.x, min.y);
    m_TexCoords[2] = glm::vec2(max.x, max.y);
    m_TexCoords[3] = glm::vec2(min.x, max.y);
  }

  Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size)
  {
    glm::vec2 min((coords.x * cell_size.x) / texture->GetWidth(), (coords.y * cell_size.y) / texture->GetHeight());
    glm::vec2 max(((coords.x + sprite_size.x) * cell_size.x) / texture->GetWidth(), ((coords.y + sprite_size.y) * cell_size.y) / texture->GetHeight());

    return CreateRef<SubTexture2D>(texture, min, max);
  }

}