#include "geometry_test.hpp"

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

}

void Triangle::move(float x, float y, float z) {
  vertex_[0].position_.translate(x,y,z);
  vertex_[1].position_.translate(x,y,z);
  vertex_[2].position_.translate(x,y,z);
}

Triangle::~Triangle() {

}