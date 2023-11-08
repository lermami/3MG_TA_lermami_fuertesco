#pragma once

#include <math.h>

class Vec3 {

public:
	Vec3();
	Vec3(float value);
	Vec3(float x, float y, float z);
	Vec3(float* values_array);
	Vec3(const float* values_array);
	Vec3(const Vec3& other);
	~Vec3();

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator+(float value) const;
	Vec3& operator+=(const Vec3& other);
	Vec3& operator+=(float value);
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator-(float value) const;
	Vec3& operator-=(const Vec3& other);
	Vec3& operator-=(float value);
	bool operator==(const Vec3& other) const;
	bool operator!=(const Vec3& other) const;
	void operator=(const Vec3& other);
	void operator=(float value);
	void operator=(const float* value);
	Vec3 operator*(float value) const;
	Vec3& operator*=(float value);
	Vec3 operator/(float value) const;
	Vec3& operator/=(float value);

	float Magnitude() const;
	Vec3 Normalized() const;
	void Normalize();
	float SqrMagnitude() const;
	void Scale(const Vec3& other);
	Vec3 CapValues(float cap);

	static Vec3 Lerp(const Vec3& a, const Vec3& b, float t);
	static Vec3 LerpUnclamped(const Vec3& a, const Vec3& b, float t);
	static float DotProduct(const Vec3& a, const Vec3& b);
	static float Angle(const Vec3& a, const Vec3& b);
	static Vec3 CrossProduct(const Vec3& a,const Vec3& b);
	static float Distance(const Vec3& a, const Vec3& b);
	static Vec3 Reflect(const Vec3& direction, const Vec3& normal);

	static const Vec3 up;
	static const Vec3 down;
	static const Vec3 right;
	static const Vec3 left;
	static const Vec3 forward;
	static const Vec3 back;
	static const Vec3 zero;
	static const Vec3 unit;

	float x;
	float y;
	float z;
};

inline float Vec3::Magnitude() const {
	return (float)sqrt((x*x)+(y*y)+(z*z));
}

inline void Vec3::Normalize() {

	float inverseMagnitude = 1.0f / Magnitude();
	*this *= inverseMagnitude;
}

inline Vec3 Vec3::Normalized() const {

	Vec3 vec3;
	vec3.x= x/Magnitude();
	vec3.y= y/Magnitude();
	vec3.z= z/Magnitude();
	return Vec3(vec3);
}

inline float Vec3::DotProduct(const Vec3& a, const Vec3& other)  {
	return (float) (a.x*other.x)+(a.y*other.y)+(a.z*other.z);
}

inline float Vec3::Angle(const Vec3& a, const Vec3& other)  {
	float d = DotProduct(a,other);
	float w = a.Magnitude()+other.Magnitude();
	return (float)acos(d/w);
}

inline Vec3 Vec3::CrossProduct(const Vec3& a, const Vec3& other)  {
	float x1,y1,z1;
	x1=(a.y*other.z)-(a.z*other.y);
	y1=(a.z*other.x)-(a.x*other.z);
	z1=(a.x*other.y)-(a.y*other.x);

	return Vec3(x1,y1,z1);
}

inline float Vec3::SqrMagnitude() const {
	return (x*x)+(y*y)+(z*z);
}

inline void Vec3::Scale(const Vec3& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

inline Vec3 Vec3::CapValues(float cap){
	if(x < cap) x = cap;
	if(y < cap) y = cap;
	if(z < cap) z = cap;

	return Vec3(x,y,z);
}

inline Vec3 Vec3::Lerp(const Vec3& a, const Vec3& b, float t) {
	if(t>1){
		t=1;
	}
	if(t<0){
		t=0;
	}
	return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

inline Vec3 Vec3::LerpUnclamped(const Vec3& a, const Vec3& b, float t) {
	return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

inline float Vec3::Distance(const Vec3& a, const Vec3& b) {
	float x1,y1,z1;
	x1= a.x-b.x;
	y1= a.y-b.y;
	z1= a.z-b.z;
	return (float) sqrt((x1*x1)+(y1*y1)+(z1*z1));
}

inline Vec3 Vec3::Reflect(const Vec3& direction, const Vec3& normal) {

	Vec3 aux;
	Vec3 n = normal.Normalized();

	aux = (2.0f * DotProduct(direction,n));
	aux.x *= n.x;
	aux.y *= n.y;
	aux.z *= n.z;

	aux = direction - aux;
	return Vec3(aux);
}

inline Vec3 Vec3::operator+(const Vec3& other) const {
	float x1,y1,z1;

	x1 = x +other.x;
	y1 = y +other.y;
	z1 = z +other.z;

	return Vec3(x1,y1,z1);
}

inline Vec3 Vec3::operator+(float value) const {
		float x1,y1,z1;
	x1 = x +value;
	y1 = y +value;
	z1 = z +value;

	return Vec3(x1,y1,z1);
}

inline Vec3& Vec3::operator+=(const Vec3& other) {
  x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

inline Vec3& Vec3::operator+=(float value) {
	x += value;
	y += value;
	z += value;
	return *this;
}

inline Vec3 Vec3::operator-(const Vec3& other) const {
	float x1,y1,z1;

	x1 = x -other.x;
	y1 = y -other.y;
	z1 = z -other.z;
	return Vec3(x1,y1,z1);
}

inline Vec3 Vec3::operator-(float value) const {
float x1,y1,z1;
x1 = x -value;
y1 = y -value;
z1 = z -value;

return Vec3(x1,y1,z1);
}

inline Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

inline Vec3& Vec3::operator-=(float value) {
	x -= value;
	y -= value;
	z -= value;
	return *this;
}

inline bool Vec3::operator==(const Vec3& other) const {
	if(x==other.x && y == other.y && z == other.z){
		return true;
	}
	return false;
}

inline bool Vec3::operator!=(const Vec3& other) const {
	if(x!=other.x && y != other.y && z != other.z){
		return true;
	}
	return false;
}

inline void Vec3::operator=(const Vec3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
}

inline void Vec3::operator=(float value) {
	x = value;
	y = value;
	z = value;
}

inline void Vec3::operator=(const float* value) {
	x = value[0];
	y = value[1];
	z = value[2];
}


inline Vec3 Vec3::operator*(float value) const {
	float x1,y1,z1;
	x1 = x *value;
	y1 = y *value;
	z1 = z *value;

	return Vec3(x1,y1,z1);
}

inline Vec3& Vec3::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

inline Vec3 Vec3::operator/(float value) const {
	float x1,y1,z1;
	x1 = x /value;
	y1 = y /value;
	z1 = z /value;

	return Vec3(x1,y1,z1);
}

inline Vec3& Vec3::operator/=(float value) {
	x /= value;
	y /= value;
	z /= value;
	return *this;
}
