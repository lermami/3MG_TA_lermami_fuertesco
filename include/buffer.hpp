#pragma once
#include <memory>
#include "enum.hpp"

class Buffer {
public:
  Buffer();
  ~Buffer();
  Buffer(const Buffer&);
  Buffer& operator=(const Buffer&);

  void init(unsigned int size);

  void bind(const Target t) const;

  unsigned int size() const;
  
  const void* data();

  void uploadData(const void* data, unsigned int size);

  void uploadFloatAttribute(unsigned int id, int size, int stride, void* offset);

  void release();

protected:
  unsigned int size_;
  unsigned int id_;
  const void* data_;

}; 

class VertexBuffer {
  friend class ResourceManager;
public:
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
  unsigned get() const;
  unsigned getCount() const;

  void uploadFloatAttribute(unsigned int id, int size, int stride, void* offset);

private:
  VertexBuffer(float* vertices, unsigned size);
  unsigned handle_;
  unsigned count_;
};