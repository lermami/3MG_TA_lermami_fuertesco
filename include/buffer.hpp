#pragma once
#include <memory>

enum Target {
  kTarget_VertexData,
  kTarget_Elements,
};

class Buffer {
public:
  Buffer();
  ~Buffer();
  Buffer(const Buffer&);
  Buffer& operator=(const Buffer&);

  void init(unsigned int size);

  void bind(const Target t) const;

  unsigned int size() const;
  
  const void* get();

  void uploadData(const void* data, unsigned int size);

  void uploadFloatAttribute(unsigned int id, int size, int stride, void* offset);

  void release();

protected:
  unsigned int size_;
  unsigned int id_;
  const void* data_;

}; 