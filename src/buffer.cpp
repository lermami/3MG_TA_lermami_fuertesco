#include "buffer.hpp"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(float* vertices, unsigned size)
{
  glCreateBuffers(1, &handle_);
  glBindBuffer(GL_ARRAY_BUFFER, handle_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  count_ = size;
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &handle_);
}

void VertexBuffer::bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, handle_);
}

void VertexBuffer::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned VertexBuffer::get() const
{
  return handle_;
}

unsigned VertexBuffer::getCount() const {
  return count_;
}

void VertexBuffer::uploadFloatAttribute(unsigned int id, int size, int stride, void* offset) {
  glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, offset);
  glBindBuffer(GL_ARRAY_BUFFER, handle_);
  glEnableVertexAttribArray(id);
}

IndexBuffer::IndexBuffer(unsigned* indices, unsigned size)
{
  glCreateBuffers(1, &handle_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  count_ = size;
}

IndexBuffer::~IndexBuffer()
{
  glDeleteBuffers(1, &handle_);
}

void IndexBuffer::bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
}

void IndexBuffer::unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned IndexBuffer::get() const
{
  return handle_;
}

unsigned IndexBuffer::getCount() const {
  return count_;
}
