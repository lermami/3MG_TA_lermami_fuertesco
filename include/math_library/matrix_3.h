//--------------------------------------------------------------//
//  Math Library
//  Matrix 3 Class Definition.
//--------------------------------------------------------------//
//
//   m0 m1 m2
//   m3 m4 m5
//   m6 m7 m8
//
//--------------------------------------------------------------//
#pragma once

#include "vector_2.h"
#include "vector_3.h"

class Mat3 {
public:

	Mat3();
	Mat3(float *values_array);
	Mat3(float value);
	Mat3(Vec3 a, Vec3 b, Vec3 c);
	Mat3(float v1, float v2, float v3, float v4, float v5, float v6,
						float v7, float v8, float v9);

	Mat3(const Mat3& copy);
	~Mat3();

	static Mat3 Identity();

	Mat3 Multiply(const Mat3& other) const;

	float Determinant() const;

	Mat3 Adjoint() const;
	bool GetInverse(Mat3& out) const;
	bool Inverse();

	Mat3 Transpose() const;

	static Mat3 Translate(const Vec2& position);
	static Mat3 Translate(float x, float y);
	static Mat3 Rotate(float angle);
	static Mat3 Scale(const Vec2& scale);
	static Mat3 Scale(float x, float y);

	Vec3 Transform_Mat3_Vec3(Mat3 mat,Vec3 vec);
	Vec2 Transform_Mat3_Vec2(Mat3 mat,Vec2 vec);

	Vec3 GetColum(int colum) const;
	Vec3 GetLine(int line) const;

	inline Mat3 operator+(const Mat3& other) const;
	inline Mat3& operator+=(const Mat3& other);
	inline Mat3 operator+(float value) const;
	inline Mat3& operator+=(float value);
	inline Mat3 operator-(const Mat3& other) const;
	inline Mat3& operator-=(const Mat3& other);
	inline Mat3 operator-(float value) const;
	inline Mat3& operator-=(float value);
	inline Mat3 operator*(float value) const;
	inline Mat3& operator*=(float value);
	inline Mat3 operator/(float value) const;
	inline Mat3& operator/=(float value);
	bool operator==(const Mat3& other) const;
	bool operator!=(const Mat3& other) const;
	inline void operator=(const Mat3& other);

	float m[9];
};


inline Mat3 Mat3::operator+(const Mat3& other) const {
	float aux[9];

	aux[0] = m[0] + other.m[0]; aux[1] = m[1] + other.m[1]; aux[2] = m[2] + other.m[2];
	aux[3] = m[3] + other.m[3]; aux[4] = m[4] + other.m[4]; aux[5] = m[5] + other.m[5];
	aux[6] = m[6] + other.m[6]; aux[7] = m[7] + other.m[7]; aux[8] = m[8] + other.m[8];

	return Mat3(aux);
}

inline Mat3& Mat3::operator+=(const Mat3& other) {

	m[0] += other.m[0]; m[1] += other.m[1]; m[2] += other.m[2];
	m[3] += other.m[3]; m[4] += other.m[4]; m[5] += other.m[5];
	m[6] += other.m[6]; m[7] += other.m[7]; m[8] += other.m[8];

	return (*this);
}

inline Mat3 Mat3::operator+(float value) const {
	float aux[9];
	aux[0] = m[0] + value; aux[1] = m[1] + value; aux[2] = m[2] + value;
	aux[3] = m[3] + value; aux[4] = m[4] + value; aux[5] = m[5] + value;
	aux[6] = m[6] + value; aux[7] = m[7] + value; aux[8] = m[8] + value;
	return Mat3(aux);
}

inline Mat3& Mat3::operator+=(float value) {
	m[0] += value; m[1] += value; m[2] += value;
	m[3] += value; m[4] += value; m[5] += value;
	m[6] += value; m[7] += value; m[8] += value;
	return (*this);
}

inline Mat3 Mat3::operator-(const Mat3& other) const {
	float aux[9];

	aux[0] = m[0] - other.m[0]; aux[1] = m[1] - other.m[1]; aux[2] = m[2] - other.m[2];
	aux[3] = m[3] - other.m[3]; aux[4] = m[4] - other.m[4]; aux[5] = m[5] - other.m[5];
	aux[6] = m[6] - other.m[6]; aux[7] = m[7] - other.m[7]; aux[8] = m[8] - other.m[8];

	return Mat3(aux);
}

inline Mat3& Mat3::operator-=(const Mat3& other) {
	m[0] -= other.m[0]; m[1] -= other.m[1]; m[2] -= other.m[2];
	m[3] -= other.m[3]; m[4] -= other.m[4]; m[5] -= other.m[5];
	m[6] -= other.m[6]; m[7] -= other.m[7]; m[8] -= other.m[8];
	return (*this);
}

inline Mat3 Mat3::operator-(float value) const {
	float aux[9];
	aux[0] = m[0] - value; aux[1] = m[1] - value; aux[2] = m[2] - value;
	aux[3] = m[3] - value; aux[4] = m[4] - value; aux[5] = m[5] - value;
	aux[6] = m[6] - value; aux[7] = m[7] - value; aux[8] = m[8] - value;
	return Mat3(aux);
}

inline Mat3& Mat3::operator-=(float value) {
	m[0] -= value; m[1] -= value; m[2] -= value;
	m[3] -= value; m[4] -= value; m[5] -= value;
	m[6] -= value; m[7] -= value; m[8] -= value;
	return (*this);
}

inline Mat3 Mat3::operator*(float value) const {
	float aux[9];
	aux[0] = m[0] * value; aux[1] = m[1] * value; aux[2] = m[2] * value;
	aux[3] = m[3] * value; aux[4] = m[4] * value; aux[5] = m[5] * value;
	aux[6] = m[6] * value; aux[7] = m[7] * value; aux[8] = m[8] * value;
	return Mat3(aux);
}

inline Mat3& Mat3::operator*=(float value) {
	m[0] *= value; m[1] *= value; m[2] *= value;
	m[3] *= value; m[4] *= value; m[5] *= value;
	m[6] *= value; m[7] *= value; m[8] *= value;
	return (*this);
}

inline Mat3 Mat3::operator/(float value) const {
	float aux[9];
	aux[0] = m[0] / value; aux[1] = m[1] / value; aux[2] = m[2] / value;
	aux[3] = m[3] / value; aux[4] = m[4] / value; aux[5] = m[5] / value;
	aux[6] = m[6] / value; aux[7] = m[7] / value; aux[8] = m[8] / value;
	return Mat3(aux);
}

inline Mat3& Mat3::operator/=(float value) {
	m[0] /= value; m[1] /= value; m[2] /= value;
	m[3] /= value; m[4] /= value; m[5] /= value;
	m[6] /= value; m[7] /= value; m[8] /= value;
	return (*this);
}

inline bool Mat3::operator==(const Mat3& other) const {
 int cont=0;
	for(int i = 0;i<9;i++){
		if(m[i]==other.m[i]){
			cont++;
		}
	}

	if(cont==9){
		return true;
	}else{
		return false;
	}

}

inline bool Mat3::operator!=(const Mat3& other) const {
	int cont=0;
	 for(int i = 0;i<9;i++){
		 if(m[i]==other.m[i]){
			 cont++;
		 }
	 }

	 if(cont!=9){
		 return true;
	 }else{
		 return false;
	 }
}

inline void Mat3::operator=(const Mat3& other) {
	m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2];
	m[3] = other.m[3]; m[4] = other.m[4]; m[5] = other.m[5];
	m[6] = other.m[6]; m[7] = other.m[7]; m[8] = other.m[8];
}

inline Mat3 Mat3::Identity() {
	Mat3 aux;
	aux.m[0] = 1.0f; aux.m[1] = 0.0f; aux.m[2] = 0.0f;
	aux.m[3] = 0.0f; aux.m[4] = 1.0f; aux.m[5] = 0.0f;
	aux.m[6] = 0.0f; aux.m[7] = 0.0f; aux.m[8] = 1.0f;

	return aux;
}

inline float Mat3::Determinant() const {


	return m[0]*m[4]*m[8] + m[1]*m[5]*m[6] +
				 m[3]*m[7]*m[2] - m[2]*m[4]*m[6] -
				 m[0]*m[5]*m[7] - m[1]*m[3]*m[8];
}

inline bool Mat3::GetInverse(Mat3& out) const {

	out = Adjoint().Transpose() / Determinant();
	return true;
}

inline bool Mat3::Inverse() {
	if(Determinant() == 0){
		return false;
	}else{
	return GetInverse((*this));

	}
}

inline Mat3 Mat3::Translate(const Vec2& mov_vector) {
	Mat3 aux;
	aux.m[0] = 1.0f; aux.m[1] = 0; aux.m[2] = mov_vector.x;
	aux.m[3] = 0; aux.m[4] = 1.0f; aux.m[5] = mov_vector.y;
	aux.m[6] = 0; aux.m[7] = 0; aux.m[8] = 1.0f;

	return Mat3(aux);
}

inline Mat3 Mat3::Translate(float x, float y) {
	Mat3 aux;
	aux.m[0] = 1.0f; aux.m[1] = 0; aux.m[2] = x;
	aux.m[3] = 0; aux.m[4] = 1.0f; aux.m[5] = y;
	aux.m[6] = 0; aux.m[7] = 0; aux.m[8] = 1.0f;
	return Mat3(aux);
}

inline Mat3 Mat3::Rotate(float angle){
	Mat3 aux;
	aux.m[0] = (float)cos(angle); aux.m[1] = (float)-sin(angle); aux.m[2] = 0;
	aux.m[3] = (float)sin(angle); aux.m[4] = (float)cos(angle); aux.m[5] = 0;
	aux.m[6] = 0; aux.m[7] = 0; aux.m[8] = 1.0f;
	return Mat3(aux);
}
inline Mat3 Mat3::Scale(const Vec2& scale){
	Mat3 aux;
	aux.m[0] = scale.x; aux.m[1] = 0; aux.m[2] = 0;
	aux.m[3] = 0; aux.m[4] = scale.y; aux.m[5] = 0;
	aux.m[6] = 0; aux.m[7] = 0; aux.m[8] = 1.0f;
	return Mat3(aux);
}

inline Mat3 Mat3::Scale(float x, float y){
	Mat3 aux;
	aux.m[0] = x; aux.m[1] = 0; aux.m[2] = 0;
	aux.m[3] = 0; aux.m[4] = y; aux.m[5] = 0;
	aux.m[6] = 0; aux.m[7] = 0; aux.m[8] = 1.0f;
	return Mat3(aux);
}

inline Vec3 Mat3::Transform_Mat3_Vec3(Mat3 mat,Vec3 vec){
	Vec3 New_vec;

	New_vec.x = (mat.m[0]*vec.x) + (mat.m[1] * vec.y) + (mat.m[2]*vec.z);
	New_vec.y = (mat.m[3]*vec.x) + (mat.m[4] * vec.y) + (mat.m[5]*vec.z);
	New_vec.z = (mat.m[6]*vec.x) + (mat.m[7] * vec.y) + (mat.m[8]*vec.z);

	return Vec3(New_vec);
}

inline Vec2 Mat3::Transform_Mat3_Vec2(Mat3 mat,Vec2 vec){
	Vec2 New_vec;

	New_vec.x = (mat.m[0]*vec.x) + (mat.m[1] * vec.y) + (mat.m[2]*1);
	New_vec.y = (mat.m[3]*vec.x) + (mat.m[4] * vec.y) + (mat.m[5]*1);;

	return Vec2(New_vec);

}

inline Mat3 Mat3::Multiply(const Mat3& other) const {
	float aux[9];

	aux[0] = (m[0] * other.m[0]) + (m[1] * other.m[3]) + (m[2] * other.m[6]);
	aux[1] = (m[0] * other.m[1]) + (m[1] * other.m[4]) + (m[2] * other.m[7]);
	aux[2] = (m[0] * other.m[2]) + (m[1] * other.m[5]) + (m[2] * other.m[8]);

	aux[3] = (m[3] * other.m[0]) + (m[4] * other.m[3]) + (m[5] * other.m[6]);
	aux[4] = (m[3] * other.m[1]) + (m[4] * other.m[4]) + (m[5] * other.m[7]);
	aux[5] = (m[3] * other.m[2]) + (m[4] * other.m[5]) + (m[5] * other.m[8]);

	aux[6] = (m[6] * other.m[0]) + (m[7] * other.m[3]) + (m[8] * other.m[6]);
	aux[7] = (m[6] * other.m[1]) + (m[7] * other.m[4]) + (m[8] * other.m[7]);
	aux[8] = (m[6] * other.m[2]) + (m[7] * other.m[5]) + (m[8] * other.m[8]);

	return Mat3(aux);
}

inline Mat3 Mat3::Adjoint() const {
	float aux[9];
	aux[0] = m[4]*m[8] - (m[7]*m[5]); aux[1] = -(m[3]*m[8] - (m[6]*m[5])); aux[2] = m[3]*m[7] - (m[6]*m[4]);
	aux[3] = -(m[1]*m[8] - (m[7]*m[2])); aux[4] = m[0]*m[8] - (m[6]*m[2]); aux[5] = -(m[0]*m[7] - (m[6]*m[1]));
	aux[6] = m[1]*m[5] - (m[4]*m[2]); aux[7] = -(m[0]*m[5] - (m[3]*m[2])); aux[8] = m[0]*m[4] - (m[3]*m[1]);
	return Mat3(aux);
}

inline Mat3 Mat3::Transpose() const {
	float aux[9];
	aux[0] = m[0]; aux[1] = m[3]; aux[2] = m[6];
	aux[3] = m[1]; aux[4] = m[4]; aux[5] = m[7];
	aux[6] = m[2]; aux[7] = m[5]; aux[8] = m[8];

	return Mat3(aux);
}

inline Vec3 Mat3::GetColum(int colum) const {
	float x,y,z;
	x = m[colum];
	y = m[colum+3];
	z = m[colum+6];
	return Vec3(x,y,z);
}

inline Vec3 Mat3::GetLine(int line) const {
	float x,y,z;
	x = m[line*3];
	y = m[line*3+1];
	z = m[line*3+2];

	return Vec3(x,y,z);
}
