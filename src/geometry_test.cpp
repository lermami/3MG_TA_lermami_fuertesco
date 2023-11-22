#include "geometry_test.hpp"

#include <GL/glew.h>
#include <string>

#include "matrix_4.hpp"
#include "shader_management.hpp"
/*
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
*/

Triangle::Triangle() {
  vertex_[0].position_ = Vec3(-0.5, -0.5, 0);
  vertex_[1].position_ = Vec3( 0.5, -0.5, 0 );
  vertex_[2].position_ = Vec3( 0, 0.5, 0 );

  vertex_[0].colors_ = Vec3( 1, 0, 0);
  vertex_[1].colors_ = Vec3( 0, 1, 0);
  vertex_[2].colors_ = Vec3(0, 0, 1);

  vertexShader_ = -1;
  fragmentShader_ = -1;
  programShader_ = -1;

  size_ = Vec2(1, 1);
  rot_ = 0;

  b_.init(sizeof(vertex_));
  b_.bind(Target::kTarget_Elements);

}

Triangle::Triangle(const char* vpath, const char* fpath) {
  vertex_[0].position_ = Vec3(-0.5, -0.5, 0);
  vertex_[1].position_ = Vec3(0.5, -0.5, 0);
  vertex_[2].position_ = Vec3(0, 0.5, 0);

  vertex_[0].colors_ = Vec3(1, 0, 0);
  vertex_[1].colors_ = Vec3(0, 1, 0);
  vertex_[2].colors_ = Vec3(0, 0, 1);

  vertexShader_ = -1;
  fragmentShader_ = -1;
  programShader_ = -1;

  size_ = Vec2(1, 1);
  rot_ = 0;

  //Buffer
  b_.init(sizeof(vertex_));
  b_.bind(Target::kTarget_Elements);

  // Program
  programShader_ = CreateProgram(vpath, fpath);
}

void Triangle::move(float x, float y, float z) {
  pos_ += Vec3(x,y,z);

}

void Triangle::roll(float radian) {
  rot_ += radian;
}

void Triangle::addSize(float x, float y) {
  size_ += Vec2(x, y);
}

void Triangle::render() {
  if(programShader_ != -1)
    glUseProgram(programShader_);


  //Transform vertices

  unsigned int total_triangles = 0;

  Mat4 m = m.Identity();
  m = m.Multiply(m.Translate(pos_.x, pos_.y, 0.0f));
  m = m.Multiply(m.RotateZ(rot_));
  m = m.Multiply(m.Scale(size_.x, size_.y, 0.0f));

  for (int i = 0; i < 3; i++) {
    transformed_vertex_[i].position_ = m.Transform_Mat4_Vec3(m, vertex_[i].position_);
    if (i+1 >= 3) {
      total_triangles += 3;
    }

    transformed_vertex_[i].colors_ = vertex_[i].colors_;
    transformed_vertex_[i].normal_ = vertex_[i].normal_;
  }


  

  b_.uploadData(transformed_vertex_, sizeof(transformed_vertex_));
  b_.uploadFloatAttribute(0, 3, 9 * sizeof(float), (void*)0);
  b_.uploadFloatAttribute(1, 3, 9 * sizeof(float), (void*)(3 * sizeof(float)));

  glDrawArrays(GL_TRIANGLES, 0, total_triangles);
}


Triangle::~Triangle() {
  glDeleteProgram(programShader_);
  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);
}