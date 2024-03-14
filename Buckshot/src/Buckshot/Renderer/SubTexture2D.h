#pragma once

#include <glm/glm.hpp>

#include "Buckshot/Core/Base.h"
#include "Buckshot/Renderer/Texture.h"

namespace Buckshot {

  class SubTexture2D 
  {
  public:
    SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

    const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
    const Ref<Texture2D> GetTexture() const { return m_Texture; }

    static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size = glm::vec2(1.0f));
  private:
    Ref<Texture2D> m_Texture;
    glm::vec2 m_TexCoords[4];
  };

}