#include "buffer.hpp"

class Vec3 {
public:
  Vec3();
  ~Vec3();
  void translate(float x, float y, float z);

private:
  float x_, y_, z_;
};

struct Vertex {
  Vec3 position_;
  Vec3 colors_;
  Vec3 normal_;
};

class Triangle {
public:
  Triangle();
  Triangle(const char* vpath, const char* fpath);
  ~Triangle();
  void move(float x, float y, float z);
  void render();

private:
  Vertex vertex_[3];

  Buffer b_;
  unsigned int vertexShader_, fragmentShader_, programShader_;
};