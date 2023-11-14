#pragma once

#include "vector_3.hpp"
#include "matrix_3.hpp"
#include <math.h>

class Vec4 {
public:

	Vec4();
	Vec4(float nx, float ny, float nz, float nw);
	Vec4(Vec3 a, float nw);
	Vec4(float a);
	Vec4(float* values_array);
	Vec4(const Vec4& other);
	~Vec4();

	Vec4 operator+(const Vec4& other) const;
	Vec4 operator+(float value) const;
	void operator+=(const Vec4& other);
	void operator+=(float value);
	Vec4 operator-(const Vec4& other) const;
	Vec4 operator-(float value) const;
	void operator -=(const Vec4& other);
	void operator -=(float value);

	Vec4 operator*(float value) const;
	void operator*=(float value);
	Vec4 operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Vec4& other);
	bool operator!=(const Vec4& other);
	void operator=(const Vec4& other);

	float Magnitude() const;
	void Normalize();
	Vec4 Normalized() const;
	void Scale(Vec4 scale);
	float SqrMagnitude() const;

	static float Distance(const Vec4& a, const Vec4& b);
	static float DotProduct(Vec4 a, Vec4 b);
	static Vec4 Lerp(const Vec4& a, const Vec4& b, float index);

	static const Vec4 one;
	static const Vec4 zero;

	float x;
	float y;
	float z;
	float w;

};


inline float Vec4::Magnitude() const{
	return (float) sqrt(((x-0) * (x-0)) + ((y - 0) * (y - 0)) + ((z - 0) * (z - 0)) + ((w - 0) * (w - 0)));
}

inline void Vec4::Normalize() {
		float magnitude = Magnitude();

		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		w /= magnitude;
}

inline Vec4 Vec4::Normalized() const {
		Vec4 aux_vec;

		aux_vec.Normalize();

		return Vec4(aux_vec);
}

inline void Vec4::Scale(Vec4 scale) {
		x *= scale.x;
		y *= scale.y;
		z *= scale.z;
		w *= scale.w;
}

inline float Vec4::SqrMagnitude() const {
	return (float) sqrt(Magnitude());
}

inline float Vec4::Distance(const Vec4& a, const Vec4& b) {
		Vec4 aux_vec;

		aux_vec.x = b.x - a.x;
		aux_vec.y = b.y - a.y;
		aux_vec.z = b.z - a.z;
		aux_vec.w = b.w - a.w;

	return aux_vec.Magnitude();
}

inline float Vec4::DotProduct(Vec4 a, Vec4 b) {
		float dot_product = ((a.x*b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));

	return dot_product;
}

inline Vec4 Vec4::Lerp(const Vec4& a, const Vec4& b, float index) {
		Vec4 aux_vec;

		if(index>1.0f) index = 1.0f;
		if(index<0.0f) index = 0.0f;

		aux_vec = (Distance(a, b) * index);

		return Vec4(aux_vec);
}

inline Vec4 Vec4::operator+(const Vec4& other) const{
		Vec4 aux_vec;

		aux_vec.x = x + other.x;
		aux_vec.y = y + other.y;
		aux_vec.z = z + other.z;
		aux_vec.w = w + other.w;

	return Vec4(aux_vec);
}

inline Vec4 Vec4::operator+(float value) const{
		Vec4 aux_vec;

		aux_vec.x = x + value;
		aux_vec.y = y + value;
		aux_vec.z = z + value;
		aux_vec.w = w + value;


		return Vec4(aux_vec);
}

inline void Vec4::operator+=(const Vec4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
}

inline void Vec4::operator+=(float value) {
		x += value;
		y += value;
		z += value;
		w += value;
}

inline Vec4 Vec4::operator-(const Vec4& other) const{
		Vec4 aux_vec;

		aux_vec.x = x - other.x;
		aux_vec.y = y - other.y;
		aux_vec.z = z - other.z;
		aux_vec.w = w - other.w;

		return Vec4(aux_vec);
}

inline Vec4 Vec4::operator-(float value) const{
		Vec4 aux_vec;

		aux_vec.x = x - value;
		aux_vec.y = y - value;
		aux_vec.z = z - value;
		aux_vec.w = w - value;

		return Vec4(aux_vec);
}

inline void Vec4::operator -=(const Vec4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
}

inline void Vec4::operator -=(float value) {
		x -= value;
		y -= value;
		z -= value;
		w -= value;
}

inline Vec4 Vec4::operator*(float value) const{
		Vec4 aux_vec;

		aux_vec.x = x * value;
		aux_vec.y = y * value;
		aux_vec.z = z * value;
		aux_vec.w = w * value;


		return Vec4(aux_vec);
}

inline void Vec4::operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;
		w *= value;
}

inline Vec4 Vec4::operator/(float value) const{
		Vec4 aux_vec;

		aux_vec.x = x / value;
		aux_vec.y = y / value;
		aux_vec.z = z / value;
		aux_vec.w = w / value;


		return Vec4(aux_vec);
}

inline void Vec4::operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;
		w /= value;
}

inline bool Vec4::operator==(const Vec4& other) {
		if (x != other.x || y != other.y || z != other.z || w != other.w)
				return false;

	return true;
}
inline bool Vec4::operator!=(const Vec4& other) {
		if (x != other.x || y != other.y || z != other.z || w != other.w)
				return true;

		return false;
}
inline void Vec4::operator=(const Vec4& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
}