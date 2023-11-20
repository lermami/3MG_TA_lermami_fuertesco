#include "buffer.hpp"
#include <GL/glew.h>

Buffer::Buffer() {
  id_ = -1;
  size_ = 0;
}

Buffer::~Buffer() {
  release();
}

Buffer::Buffer(const Buffer&) {}
Buffer& Buffer::operator=(const Buffer&) { return *this; }

void Buffer::init(unsigned int size) {
  glGenBuffers(1, &id_);
  bind(Target::kTarget_VertexData);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

  size_ = size;
}

void Buffer::bind(const Target t) const {
  GLenum type = 0;

  switch (t) {
  case Target::kTarget_VertexData:
    type = GL_ARRAY_BUFFER;
    break;

  case Target::kTarget_Elements:
    type = GL_ELEMENT_ARRAY_BUFFER;
    break;
  }
  glBindBuffer(type, id_);
}

unsigned int Buffer::size() const {
  return size_;
}

void Buffer::uploadData(const void* data, unsigned int size) {
  bind(Target::kTarget_VertexData);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void Buffer::uploadFloatAttribute(unsigned int id, int size, int stride, void* offset) {
  glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, offset);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glEnableVertexAttribArray(id);
}

void Buffer::release() {
  if (id_ != -1) {
    glDeleteBuffers(1, &id_);
    id_ = -1;
    size_ = 0;
  }
}