#include <bspch.h>
#undef INFINITE
#include <msdf-atlas-gen.h>

#include "Buckshot/Renderer/Font.h"

namespace Buckshot {

  Font::Font(const std::filesystem::path& filepath)
  {
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    if (ft)
    {
      std::string filename = filepath.string();
      msdfgen::FontHandle* fh = msdfgen::loadFont(ft, filename.c_str());
      if (fh)
      {
        msdfgen::Shape shape;
        if (msdfgen::loadGlyph(shape, fh, 'A'))
        {
          shape.normalize();
          msdfgen::edgeColoringSimple(shape, 3.0f);
          msdfgen::Bitmap<float, 3> msdf(32, 32);
          msdfgen::generateMSDF(msdf, shape, 4.0f, 1.0f, msdfgen::Vector2(4.0f, 4.0f));
          msdfgen::savePng(msdf, "output.png");
        }
        msdfgen::destroyFont(fh);
      }
    }
    msdfgen::deinitializeFreetype(ft);
  }

}