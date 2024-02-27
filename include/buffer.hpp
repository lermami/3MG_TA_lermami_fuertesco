#pragma once
#include <memory>
#include "enum.hpp"

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

class IndexBuffer {
  friend class ResourceManager;
public:
  ~IndexBuffer();

  void bind() const;
  void unbind() const;
  unsigned get() const;
  unsigned getCount() const;

private:
  IndexBuffer(unsigned* vertices, unsigned size);
  unsigned handle_;
  unsigned count_;
};