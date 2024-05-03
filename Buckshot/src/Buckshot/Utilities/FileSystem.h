#pragma once

#include "Buckshot/Utilities/Buffer.h"

namespace Buckshot
{
  class FileSystem
  {
	public:
    static Buffer ReadFileBinary(const std::filesystem::path& filepath);
  };
}