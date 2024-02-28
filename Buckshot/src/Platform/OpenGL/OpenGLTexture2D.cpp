#include <bspch.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Buckshot {

  OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
  {
    m_Path = path;

    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    BS_ASSERT(data, "Failed to load image \"{0}\"", path);
    m_Width = width;
    m_Height = height;

    GLenum internal_format = 0;
    GLenum data_format = 0;
    
    switch (channels)
    {
    case 4:
      internal_format = GL_RGBA8;
      data_format = GL_RGBA;
      break;
    case 3:
      internal_format = GL_RGB8;
      data_format = GL_RGB;
      break;
    default:
      BS_ERROR("When loading the image \"{0}\", encountered an unknown color format \"{1}\"", path, channels);
      break;
    }

    BS_ASSERT(internal_format && data_format, "Incorrectly specified the color format for the image \"{0}\"", path);
    BS_ASSERT(internal_format, "Internal Format : {0}", internal_format);
    BS_ASSERT(data_format, "Data Format : {0}", data_format);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internal_format, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, data_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  OpenGLTexture2D::~OpenGLTexture2D()
  {
    glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const
  {
    glBindTextureUnit(slot, m_RendererID);
  }

}