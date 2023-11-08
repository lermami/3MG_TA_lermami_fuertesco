#include "geometry_test.hpp"

#include <GL/glew.h>
#include <string>

#include "shader_management.hpp"

Vec3::Vec3() {
  x_ = 0;
  y_ = 0;
  z_ = 0;
}

void Vec3::translate(float x, float y, float z) {
  x_ += x;
  y_ += y;
  z_ += z;
}

Vec3::~Vec3(){

}

Triangle::Triangle() {
  vertex_[0].position_.translate(-0.5, -0.5, 0);
  vertex_[1].position_.translate(0.5, -0.5, 0);
  vertex_[2].position_.translate(0, 0.5, 0);

  vertex_[0].colors_.translate(1, 0, 0);
  vertex_[1].colors_.translate(0, 1, 0);
  vertex_[2].colors_.translate(0, 0, 1);

  vertexShader_ = -1;
  fragmentShader_ = -1;
  programShader_ = -1;

  b_.init(sizeof(vertex_));
  b_.bind(Target::kTarget_Elements);

}

Triangle::Triangle(const char* vpath, const char* fpath) {
  vertex_[0].position_.translate(-0.5, -0.5, 0);
  vertex_[1].position_.translate(0.5, -0.5, 0);
  vertex_[2].position_.translate(0, 0.5, 0);

  vertex_[0].colors_.translate(1, 0, 0);
  vertex_[1].colors_.translate(0, 1, 0);
  vertex_[2].colors_.translate(0, 0, 1);

  vertexShader_ = -1;
  fragmentShader_ = -1;
  programShader_ = -1;

  //Buffer
  b_.init(sizeof(vertex_));
  b_.bind(Target::kTarget_Elements);

  // Shaders
  std::string v = ReadFiles(vpath);
  std::string f = ReadFiles(fpath);

  vertexShader_ = CreateShader(0);
  CompileShader(vertexShader_, v.c_str());
  fragmentShader_ = CreateShader(1);
  CompileShader(fragmentShader_, f.c_str());

  // Program
  programShader_ = CreateProgram(vertexShader_, fragmentShader_);
}

void Triangle::move(float x, float y, float z) {
  vertex_[0].position_.translate(x,y,z);
  vertex_[1].position_.translate(x,y,z);
  vertex_[2].position_.translate(x,y,z);
}

void Triangle::render() {
  if(programShader_ != -1)
    glUseProgram(programShader_);

  b_.uploadData(vertex_, sizeof(vertex_));
  b_.uploadFloatAttribute(0, 3, 9 * sizeof(float), (void*)0);
  b_.uploadFloatAttribute(1, 3, 9 * sizeof(float), (void*)(3 * sizeof(float)));

  glDrawArrays(GL_TRIANGLES, 0, 3);
}


Triangle::~Triangle() {
  glDeleteProgram(programShader_);
  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);
}