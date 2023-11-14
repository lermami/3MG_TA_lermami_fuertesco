#include "matrix_3.hpp"

Mat3::Mat3() {
  m[0] = 0.0f; m[1] = 0.0f; m[2] = 0.0f;
  m[3] = 0.0f; m[4] = 0.0f; m[5] = 0.0f;
  m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f;
}

Mat3::Mat3(float value) {
  m[0] = value; m[1] = value; m[2] = value;
  m[3] = value; m[4] = value; m[5] = value;
  m[6] = value; m[7] = value; m[8] = value;
}

Mat3::Mat3(float* values_array) {
  m[0] = values_array[0]; m[1] = values_array[1]; m[2] = values_array[2];
  m[3] = values_array[3]; m[4] = values_array[4]; m[5] = values_array[5];
  m[6] = values_array[6]; m[7] = values_array[7]; m[8] = values_array[8];
}

Mat3::Mat3(float v1, float v2, float v3, float v4, float v5, float v6, 
           float v7, float v8, float v9) {
  m[0] = v1; m[1] = v2; m[2] = v3;
  m[3] = v4; m[4] = v5; m[5] = v6;
  m[6] = v7; m[7] = v8; m[8] = v9;
}

Mat3::Mat3(Vec3 a, Vec3 b, Vec3 c) {
  m[0] =a.x; m[1] =a.y; m[2] =a.z;
  m[3] =b.x; m[4] =b.y; m[5] =b.z;
  m[6] =c.x; m[7] =c.y; m[8] =c.z;
}

Mat3::Mat3(const Mat3& copy) {
  m[0] = copy.m[0]; m[1] = copy.m[1]; m[2] = copy.m[2];
  m[3] = copy.m[3]; m[4] = copy.m[4]; m[5] = copy.m[5];
  m[6] = copy.m[6]; m[7] = copy.m[7]; m[8] = copy.m[8];
}

Mat3::~Mat3() {}
