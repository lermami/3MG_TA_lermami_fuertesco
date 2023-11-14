#pragma once

#include "vector_3.hpp"
#include "vector_4.hpp"
#include "matrix_3.hpp"
#include <math.h>
#include "stdio.h"

class Mat4 {
 public:

  Mat4();
  Mat4(float a[16]);
  Mat4(float value);
  Mat4(const Mat4& copy);
  ~Mat4();

  static Mat4 Identity();
  Mat4 Multiply(const Mat4& other) const;

  float Determinant() const;
  Mat4 Adjoint() const;
  bool GetInverse(Mat4* out) const;
  bool Inverse();

  Mat4 Transpose() const;

  static Mat4 Translate(const Vec3& distance);
  static Mat4 Translate(float x, float y, float z);

  static Mat4 Scale(const Vec3& scale);
  static Mat4 Scale(float x, float y, float z);

  static Mat4 RotateX(float radians);
  static Mat4 RotateY(float radians);
  static Mat4 RotateZ(float radians);

  inline Mat4 GetTransform(const Vec3& translate, const Vec3& scale,
                                 float rotateX, float rotateY, float rotateZ);

  static Mat4 GetTransform(float trans_x, float trans_y, float trans_z,
                      float scale_x, float scale_y, float scale_Z,
                      float rotateX, float rotateY, float rotateZ);

  Vec4 Transform_Mat4_Vec4(Mat4 mat,Vec4 vec);
  Vec3 Transform_Mat4_Vec3(Mat4 mat,Vec3 vec);

  Mat4 PerspectiveMatrix(float fov, float aspect,
	  float near, float far) const;

  Mat4 OrthoMatrix(float right, float left, float top, float valueottom,
	  float near, float far) const;

  Vec4 GetColum(int colum) const;
  Vec4 GetLine(int line) const;

  Mat4 operator+(const Mat4& other) const;
  Mat4& operator+=(const Mat4& other);
  Mat4 operator+(float value) const;
  Mat4& operator+=(float value);
  Mat4 operator-(const Mat4& other) const;
  Mat4& operator-=(const Mat4& other);
  Mat4 operator-(float value) const;
  Mat4& operator-=(float value);
  Mat4& operator*=(float value);
  Mat4 operator*(float value) const;
  Mat4& operator/=(float value);
  Mat4 operator/(float value) const;
  bool operator==(const Mat4& other);
  bool operator!=(const Mat4& other);
  void operator=(const Mat4& other);

  float m[16];
};


inline Mat4 Mat4::Identity() {
  float idn[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	return Mat4(idn);
}

inline Mat4 Mat4::Multiply(const Mat4& other)const  {
  Mat4 res;

  res.m[0] =  m[0] * other.m[0] + m[1] * other.m[4] + m[2] * other.m[8] + m[3] * other.m[12];
  res.m[1] =  m[0] * other.m[1] + m[1] * other.m[5] + m[2] * other.m[9] + m[3] * other.m[13];
  res.m[2] =  m[0] * other.m[2] + m[1] * other.m[6] + m[2] * other.m[10] + m[3] * other.m[14];
  res.m[3] =  m[0] * other.m[3] + m[1] * other.m[7] + m[2] * other.m[11] + m[3] * other.m[15];

  res.m[4] =  m[4] * other.m[0] + m[5] * other.m[4] + m[6] * other.m[8] + m[7] * other.m[12];
  res.m[5] =  m[4] * other.m[1] + m[5] * other.m[5] + m[6] * other.m[9] + m[7] * other.m[13];
  res.m[6] =  m[4] * other.m[2] + m[5] * other.m[6] + m[6] * other.m[10] + m[7] * other.m[14];
  res.m[7] =  m[4] * other.m[3] + m[5] * other.m[7] + m[6] * other.m[11] + m[7] * other.m[15];

  res.m[8] =  m[8] * other.m[0] + m[9] * other.m[4] + m[10] * other.m[8] + m[11] * other.m[12];
  res.m[9] =  m[8] * other.m[1] + m[9] * other.m[5] + m[10] * other.m[9] + m[11] * other.m[13];
  res.m[10] = m[8] * other.m[2] + m[9] * other.m[6] + m[10] * other.m[10] + m[11] * other.m[14];
  res.m[11] = m[8] * other.m[3] + m[9] * other.m[7] + m[10] * other.m[11] + m[11] * other.m[15];

  res.m[12] = m[12] * other.m[0] + m[13] * other.m[4] + m[14] * other.m[8] + m[15] * other.m[12];
  res.m[13] = m[12] * other.m[1] + m[13] * other.m[5] + m[14] * other.m[9] + m[15] * other.m[13];
  res.m[14] = m[12] * other.m[2] + m[13] * other.m[6] + m[14] * other.m[10] + m[15] * other.m[14];
  res.m[15] = m[12] * other.m[3] + m[13] * other.m[7] + m[14] * other.m[11] + m[15] * other.m[15];

	return res;
}

inline float Mat4::Determinant() const {
  Mat4 mat_aux(*this);
  bool change_sign = false;

  float aux = 0;
  if (mat_aux.m[4] != 0) {
    aux = mat_aux.m[4] / mat_aux.m[0];
    for (int i = 0; i < 4; i++) {
      mat_aux.m[i + 4] -= aux * mat_aux.m[i];
    }
  }

  if (mat_aux.m[8] != 0) {
    aux = mat_aux.m[8] / mat_aux.m[0];
    for (int i = 0; i < 4; i++) {
      mat_aux.m[i + 8] -= aux * mat_aux.m[i];
    }
  }

  if (mat_aux.m[12] != 0) {
    aux = mat_aux.m[12] / mat_aux.m[0];
    for (int i = 0; i < 4; i++) {
      mat_aux.m[i + 12] -= aux * mat_aux.m[i];
    }
  }

  if (mat_aux.m[5] == 0) {
    if (mat_aux.m[9] == 0 && mat_aux.m[13] == 0) {
      return 0; 
    }
    else if(mat_aux.m[9] != 0){
      Vec4 row_aux(GetLine(1));
      mat_aux.m[5] = mat_aux.m[9];
      mat_aux.m[6] = mat_aux.m[10];
      mat_aux.m[7] = mat_aux.m[11];

      mat_aux.m[9] = row_aux.y;
      mat_aux.m[10] = row_aux.z;
      mat_aux.m[11] = row_aux.w;
      change_sign = true;
    }
    else if (mat_aux.m[13] != 0) {
      Vec4 row_aux(GetLine(1));;
      mat_aux.m[5] = mat_aux.m[13];
      mat_aux.m[6] = mat_aux.m[14];
      mat_aux.m[7] = mat_aux.m[15];

      mat_aux.m[13] = row_aux.y;
      mat_aux.m[14] = row_aux.z;
      mat_aux.m[15] = row_aux.w;
      change_sign = true;
    }
  }

  if (mat_aux.m[9] != 0) {
    aux = mat_aux.m[9] / mat_aux.m[5];
    for (int i = 5; i < 8; i++) {
      mat_aux.m[i + 4] -= aux * mat_aux.m[i];
    }
  }

  if (mat_aux.m[13] != 0) {
    aux = mat_aux.m[13] / mat_aux.m[5];
    for (int i = 5; i < 8; i++) {
      mat_aux.m[i + 8] -= aux * mat_aux.m[i];
    }
  }

  if (mat_aux.m[14] != 0) {
    aux = mat_aux.m[14] / mat_aux.m[10];
    for (int i = 10; i < 12; i++) {
      mat_aux.m[i + 4] -= aux * mat_aux.m[i];
    }
  }

  if (change_sign) {
    return -(mat_aux.m[0] * mat_aux.m[5] * mat_aux.m[10] * mat_aux.m[15]);
  }
  else {
    return mat_aux.m[0] * mat_aux.m[5] * mat_aux.m[10] * mat_aux.m[15];
  }
}

inline Mat4 Mat4::Adjoint() const {
	Mat4 result;

  result.m[0] = Mat3(m[5], m[6], m[7],
                     m[9], m[10], m[11],
                     m[13], m[14], m[15]).Determinant();
  result.m[1] = -Mat3(m[4], m[6], m[7],
                     m[8], m[10], m[11],
                     m[12], m[14], m[15]).Determinant();
  result.m[2] = Mat3(m[4], m[5], m[7],
                     m[8], m[9], m[11],
                     m[12], m[13], m[15]).Determinant();
  result.m[3] = -Mat3(m[4], m[5], m[6],
                     m[8], m[9], m[10],
                     m[12], m[13], m[14]).Determinant();
  result.m[4] = -Mat3(m[1], m[2], m[3],
                     m[9], m[10], m[11],
                     m[13], m[14], m[15]).Determinant();
  result.m[5] = Mat3(m[0], m[2], m[3],
                     m[8], m[10], m[11],
                     m[12], m[14], m[15]).Determinant();
  result.m[6] = -Mat3(m[0], m[1], m[3],
                     m[8], m[9], m[11],
                     m[12], m[13], m[15]).Determinant();
  result.m[7] = Mat3(m[0], m[1], m[2],
                     m[8], m[9], m[10],
                     m[12], m[13], m[14]).Determinant();
  result.m[8] = Mat3(m[1], m[2], m[3],
                     m[5], m[6], m[7],
                     m[13], m[14], m[15]).Determinant();
  result.m[9] = -Mat3(m[0], m[2], m[3],
                     m[4], m[6], m[7],
                     m[12], m[14], m[15]).Determinant();
  result.m[10] = Mat3(m[0], m[1], m[3],
                      m[4], m[5], m[7],
                      m[12], m[13], m[15]).Determinant();
  result.m[11] = -Mat3(m[0], m[1], m[2],
                      m[4], m[5], m[6],
                      m[12], m[13], m[14]).Determinant();
  result.m[12] = -Mat3(m[1], m[2], m[3],
                      m[5], m[6], m[7],
                      m[9], m[10], m[11]).Determinant();
  result.m[13] = Mat3(m[0], m[2], m[3],
                      m[4], m[6], m[7],
                      m[8], m[10], m[11]).Determinant();
  result.m[14] = -Mat3(m[0], m[1], m[3],
                      m[4], m[5], m[7],
                      m[8], m[9], m[11]).Determinant();
  result.m[15] = Mat3(m[0], m[1], m[2],
                      m[4], m[5], m[6],
                      m[8], m[9], m[10]).Determinant();


	return result;
}

inline bool Mat4::Inverse() {
  if (Determinant() == 0) {
    return false;
  }
  else {
    return GetInverse(this);
  }
}

inline bool Mat4::GetInverse(Mat4* out) const {
  *out = Adjoint().Transpose() / Determinant();
  return true;
}

inline Mat4 Mat4::Transpose() const {
  Mat4 t;
  t.m[0] = m[0];
  t.m[1] = m[4];
  t.m[2] = m[8];
  t.m[3] = m[12];
  t.m[4] = m[1];
  t.m[5] = m[5];
  t.m[6] = m[9];
  t.m[7] = m[13];
  t.m[8] = m[2];
  t.m[9] = m[6];
  t.m[10] = m[10];
  t.m[11] = m[14];
  t.m[12] = m[3];
  t.m[13] = m[7];
  t.m[14] = m[11];
  t.m[15] = m[15];
  return t;
}

inline Mat4 Mat4::Translate(const Vec3& distance){
  Mat4 res;
  res.m[0] = 1;  res.m[1] = 0;  res.m[2] = 0;  res.m[3] = distance.x;
  res.m[4] = 0;  res.m[5] = 1;  res.m[6] = 0;  res.m[7] = distance.y;
  res.m[8] = 0;  res.m[9] = 0;  res.m[10] = 1; res.m[11] = distance.z;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
	return res;
}

inline Mat4 Mat4::Translate(float x, float y, float z){
  Mat4 res;
  res.m[0] = 1;  res.m[1] = 0;  res.m[2] = 0;  res.m[3] = x;
  res.m[4] = 0;  res.m[5] = 1;  res.m[6] = 0;  res.m[7] = y;
  res.m[8] = 0;  res.m[9] = 0;  res.m[10] = 1; res.m[11] = z;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
	return res;
}

inline Mat4 Mat4::Scale(const Vec3& scale){
  Mat4 res;
  res.m[0] = scale.x;  res.m[1] = 0;  res.m[2] = 0;  res.m[3] = 0;
  res.m[4] = 0;  res.m[5] = scale.y;  res.m[6] = 0;  res.m[7] = 0;
  res.m[8] = 0;  res.m[9] = 0;  res.m[10] = scale.z; res.m[11] = 0;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
	return res;
}

inline Mat4 Mat4::Scale(float x, float y, float z){
  Mat4 res;
  res.m[0] = x;  res.m[1] = 0;  res.m[2] = 0;  res.m[3] = 0;
  res.m[4] = 0;  res.m[5] = y;  res.m[6] = 0;  res.m[7] = 0;
  res.m[8] = 0;  res.m[9] = 0;  res.m[10] = z; res.m[11] = 0;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
  return res;
}

inline Mat4 Mat4::RotateX(float radians){
  Mat4 res;
  res.m[0] = 1;  res.m[1] = 0;  res.m[2] = 0;  res.m[3] = 0;
  res.m[4] = 0;  res.m[5] = (float)cos(radians);  res.m[6] = (float)-sin(radians);  res.m[7] = 0;
  res.m[8] = 0;  res.m[9] = (float)sin(radians);  res.m[10] = (float)cos(radians); res.m[11] = 0;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
  return res;

}

inline Mat4 Mat4::RotateY(float radians){
  Mat4 res;
  res.m[0] = (float)cos(radians);  res.m[1] = 0;  res.m[2] = (float)sin(radians);  res.m[3] = 0;
  res.m[4] = 0;  res.m[5] = 1;  res.m[6] =0;  res.m[7] = 0;
  res.m[8] = (float)-sin(radians);  res.m[9] = 0;  res.m[10] = (float)cos(radians); res.m[11] = 0;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
  return res;
}

inline Mat4 Mat4::RotateZ(float radians) {
  Mat4 res;
  res.m[0] = (float)cos(radians);  res.m[1] = (float)-sin(radians);  res.m[2] = 0;  res.m[3] = 0;
  res.m[4] = (float)sin(radians);  res.m[5] = (float)cos(radians);  res.m[6] = 0;  res.m[7] = 0;
  res.m[8] = 0;  res.m[9] = 0;  res.m[10] = 1; res.m[11] = 0;
  res.m[12] = 0; res.m[13] = 0; res.m[14] = 0; res.m[15] = 1;
  return res;
}

inline Mat4 Mat4::GetTransform(const Vec3& translate, const Vec3& scale,
                               float rotateX, float rotateY, float rotateZ){

    Mat4 result = result.RotateX(rotateX).Multiply(
                  result.RotateY(rotateY).Multiply(
                  result.RotateZ(rotateZ)));

    result = result.Translate(translate).Multiply(
             result.Multiply(result.Scale(scale)));

    return result;
}

inline Mat4 Mat4::GetTransform(float trans_x, float trans_y, float trans_z,
	float scale_x, float scale_y, float scale_Z,
	float rotateX, float rotateY, float rotateZ)  {

    Mat4 result = result.RotateX(rotateX).Multiply(
                  result.RotateY(rotateY).Multiply(
                  result.RotateZ(rotateZ)));

    result = result.Translate(trans_x, trans_y, trans_z).Multiply(
             result.Multiply(result.Scale(scale_x, scale_y, scale_Z)));

    return result;

}

inline Vec4 Mat4::Transform_Mat4_Vec4(Mat4 mat, Vec4 vec){
  Vec4 New_vec;

  New_vec.x = (mat.m[0]*vec.x) + (mat.m[1] * vec.y) + (mat.m[2]*vec.z) + (mat.m[3]*vec.w);
  New_vec.y = (mat.m[4]*vec.x) + (mat.m[5] * vec.y) + (mat.m[6]*vec.z) + (mat.m[7]*vec.w);
  New_vec.z = (mat.m[8]*vec.x) + (mat.m[9] * vec.y) + (mat.m[10]*vec.z) + (mat.m[11]*vec.w);
  New_vec.w = (mat.m[12]*vec.x) + (mat.m[13] * vec.y) + (mat.m[14]*vec.z) + (mat.m[15]*vec.w);

  return Vec4(New_vec);
}

inline Vec3 Mat4::Transform_Mat4_Vec3(Mat4 mat,Vec3 vec){
  Vec4 new_vec = Vec4(vec.x, vec.y, vec.z, 1.0f);

  new_vec = Transform_Mat4_Vec4(mat, new_vec);

  return Vec3(new_vec.x, new_vec.y, new_vec.z);
}

inline Vec4 Mat4::GetColum(int colum) const {
	return Vec4(m[4 * colum], m[4 + 4*colum], m[8 + 4 * colum], m[12 + 4 * colum]);
}

inline Vec4 Mat4::GetLine(int line) const {
  return Vec4(m[4 * line], m[1 + 4 * line], m[2 + 4 * line], m[3 + 4 * line]);
}

inline Mat4 Mat4::PerspectiveMatrix(float fov, float aspect,
	float near, float far) const {

  Mat4 res;
  res.m[0] = aspect * (float)(1 / tan(fov / 2));
  res.m[5] = 1 / (float)tan(fov / 2);
  res.m[10] = far / (far - near);
  res.m[11] = (-far * near) / (far - near);
  res.m[14] = 1.0f;

	return res;
}

//TODO: implement function
inline Mat4 Mat4::OrthoMatrix(float right, float left, float top, float valueottom,
	float near, float far) const {
  right; left; top; valueottom; near; far;
	return Mat4();
}

inline Mat4 Mat4::operator+(const Mat4& other) const {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] + other.m[i];
  }

	return res;
}

inline Mat4& Mat4::operator+=(const Mat4& other) {

  for(int i=0;i<16;i++){
    m[i] += other.m[i];
  }

	return (*this);
}

inline Mat4 Mat4::operator+(float value) const {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] + value;
  }
	return res;
}

inline Mat4& Mat4::operator+=(float value) {
  for(int i=0;i<16;i++){
    m[i] += value;
  }

	return (*this);
}

inline Mat4 Mat4::operator-(const Mat4& other) const  {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] - other.m[i];
  }

	return res;
}

inline Mat4& Mat4::operator-=(const Mat4& other) {
  for(int i=0;i<16;i++){
    m[i] -= other.m[i];
  }

	return (*this);
}

inline Mat4 Mat4::operator-(float value) const  {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] - value;
  }
	return res;
}

inline Mat4& Mat4::operator-=(float value) {
  for(int i=0;i<16;i++){
    m[i] -= value;
  }

	return (*this);
}

inline Mat4& Mat4::operator*=(float value) {
  for(int i=0;i<16;i++){
    m[i] *= value;
  }

	return (*this);
}

inline Mat4 Mat4::operator*(float value) const  {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] * value;
  }
	return res;
}

inline Mat4& Mat4::operator/=(float value) {
  for(int i=0;i<16;i++){
    m[i] /= value;
  }

	return (*this);
}

inline Mat4 Mat4::operator/(float value) const {
  Mat4 res;

  for(int i=0;i<16;i++){
    res.m[i] = m[i] / value;
  }

	return res;
}

inline bool Mat4::operator==(const Mat4& other) {


	return m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2] && m[3] == other.m[3] &&
         m[4] == other.m[4] && m[5] == other.m[5] && m[6] == other.m[6] && m[7] == other.m[7] &&
         m[8] == other.m[8] && m[9] == other.m[9] && m[10] == other.m[10] && m[11] == other.m[11] &&
         m[12] == other.m[12] && m[13] == other.m[13] && m[14] == other.m[14] && m[15] == other.m[15];
}

inline bool Mat4::operator!=(const Mat4& other) {

  return m[0] != other.m[0] || m[1] != other.m[1] || m[2] != other.m[2] || m[3] != other.m[3] ||
         m[4] != other.m[4] || m[5] != other.m[5] || m[6] != other.m[6] || m[7] != other.m[7] ||
         m[8] != other.m[8] || m[9] != other.m[9] || m[10] != other.m[10] || m[11] != other.m[11] ||
         m[12] != other.m[12] || m[13] != other.m[13] || m[14] != other.m[14] || m[15] != other.m[15];
}

inline void Mat4::operator=(const Mat4& other) {
  for(int i=0;i<16;i++){
    m[i] = other.m[i];
  }
}
