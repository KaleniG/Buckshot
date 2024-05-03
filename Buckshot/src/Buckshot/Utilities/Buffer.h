#pragma once

#include <memory>

namespace Buckshot {

  struct Buffer
  {
    uint8_t* Data = nullptr;
    uint64_t Size = 0;

    Buffer() = default;
    Buffer(const Buffer&) = default;
    Buffer(uint64_t size) { Allocate(size); }
    ~Buffer() = default;

    static Buffer Copy(Buffer& other)
    {
      Buffer result(other.Size);
      std::memcpy(&result, &other, sizeof(Buffer));
      return result;
    }

    void Reallocate(uint64_t size)
    {
      Release();
      Allocate(size);
    }

    void Release()
    {
      delete[] Data;
      Data = nullptr;
      Size = 0;
    }

    template<typename T>
    T* As()
    {
      return (T*)Data;
    }

    operator bool() const
    {
      return Data;
    }

  private:
    void Allocate(uint64_t size)
    {
      Size = size;
      Data = new uint8_t[size];
    }
  };

  struct ScopedBuffer
  {
    uint8_t* Data = nullptr;
    uint64_t Size = 0;

    ScopedBuffer() = default;
    ScopedBuffer(const ScopedBuffer&) = default;
    ScopedBuffer(const Buffer& other) : Data(other.Data), Size(other.Size) {}
    ScopedBuffer(uint64_t size) { Allocate(size); }
    ~ScopedBuffer() { delete[] Data; }

    void Reallocate(uint64_t size)
    {
      Release();
      Allocate(size);
    }

    void Release()
    {
      delete[] Data;
      Data = nullptr;
      Size = 0;
    }

    template<typename T>
    T* As()
    {
      return (T*)Data;
    }

    operator bool() const
    {
      return Data;
    }

  private:
    void Allocate(uint64_t size)
    {
      Size = size;
      Data = new uint8_t[size];
    }
  };

}