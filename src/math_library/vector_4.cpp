#include "vector_4.hpp"

const Vec4 Vec4::one = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
const Vec4 Vec4::zero = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

Vec4::Vec4() {
  x = 0.0f;
	y = 0.0f;
	z = 0.0f;
  w = 0.0f;
}

Vec4::Vec4(float nx, float ny, float nz, float nw) {
  x = nx;
  y = ny;
  z = nz;
  w = nw;
}

Vec4::Vec4(Vec3 a, float nw) {
  x = a.x;
  y = a.y;
  z = a.z;
  w = nw;
}

Vec4::Vec4(float a) {
  x = a;
  y = a;
  z = a;
  w = a;
}

Vec4::Vec4(float* values_array) {
  x = values_array[0];
  y = values_array[1];
  z = values_array[2];
  w = values_array[3];
}

Vec4::Vec4(const Vec4& other) {
  x = other.x;
  y = other.y;
  z = other.z;
  w = other.w;
}

Vec4::~Vec4() { }
