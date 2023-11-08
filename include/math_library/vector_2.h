#pragma once

#include <math.h>

class Vec2 {
 public:

  Vec2();
  Vec2(float x, float y);
  Vec2(const Vec2& copy);
  ~Vec2();

  Vec2 operator+(const Vec2& other) const;
  Vec2 operator+(float value);
  Vec2& operator+=(const Vec2& other);
  Vec2& operator+=(float value);
  Vec2 operator-(const Vec2& other) const;
  Vec2 operator-(float value) const;
  Vec2& operator-();
  Vec2& operator-=(const Vec2& other);
  Vec2& operator-=(float value);
  bool operator==(const Vec2& other) const;
  bool operator!=(const Vec2& other) const;
  void operator=(const Vec2& other);
  void operator=(float value);
  Vec2 operator*(float value) const;
  Vec2& operator*=(float value);
  Vec2 operator/(float value) const;
  Vec2& operator/=(float value);

  float Magnitude() const;
  void Normalize();
  Vec2 Normalized() const;

  void Scale(const Vec2 scale);

  float SqrMagnitude() const;
  static float Distance(const Vec2 a, const Vec2 b);

  static float DotProduct(Vec2 a, Vec2 b);

  static Vec2 Lerp(const Vec2 a, const Vec2 b, float t);
  static Vec2 LerpUnclamped(const Vec2 a, const Vec2 b, float t);

  static const Vec2 up;
  static const Vec2 down;
  static const Vec2 right;
  static const Vec2 left;
  static const Vec2 zero;
  static const Vec2 one;

  float x;
  float y;
};


inline Vec2 Vec2::operator+(const Vec2& other) const {
  return Vec2(x+other.x, y+other.y);
}

inline Vec2 Vec2::operator+(float value) {
  return Vec2(x+value, y+value);
}

inline Vec2& Vec2::operator+=(const Vec2& other){
  x += other.x;
  y += other.y;
  return *this;
}

inline Vec2& Vec2::operator+=(float value){
  x += value;
  y += value;
  return *this;
}

inline Vec2 Vec2::operator-(const Vec2& other) const {
  return Vec2(x - other.x, y - other.y);
}

inline Vec2 Vec2::operator-(float value) const {
  return Vec2(x - value, y - value);
}

inline Vec2& Vec2::operator-() {
  x = -x;
  y = -y;
	return *this;
}

inline Vec2& Vec2::operator-=(const Vec2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

inline Vec2& Vec2::operator-=(float value){
  x -= value;
  y -= value;
  return *this;
}

inline bool Vec2::operator==(const Vec2& value) const {
  return x==value.x && y==value.y;
}

inline bool Vec2::operator!=(const Vec2& value) const {
  return x != value.x || y != value.y;
}


inline void Vec2::operator=(const Vec2& other) {
  x = other.x;
  y = other.y;
}

inline void Vec2::operator=(float value) {
  x = value;
  y = value;
}

inline Vec2 Vec2::operator*(float value) const {
	return Vec2(x*value, y*value);
}

inline Vec2& Vec2::operator*=(float value) {
  x *= value;
  y *= value;
  return *this;
}

inline Vec2 Vec2::operator/(float value) const {
	return Vec2(x/value, y/value);
}

inline Vec2& Vec2::operator/=(float value) {
  x /= value;
  y /= value;
	return *this;
}

inline float Vec2::Magnitude() const {

  return (float)sqrt((x * x) + (y * y));
}

inline void Vec2::Normalize() {
  float m = Magnitude();
  x /= m;
  y /= m;
}

inline Vec2 Vec2::Normalized() const {
  float m = Magnitude();
  return Vec2(x/m , y/m);
}

inline void Vec2::Scale(const Vec2 scale){
  x *= scale.x;
  y *= scale.y;
}

inline float Vec2::SqrMagnitude() const {
  return (x * x) + (y * y);
}


inline float Vec2::Distance(const Vec2 a, const Vec2 b) {

  return Vec2(a-b).Magnitude();
}

inline Vec2 Vec2::Lerp(const Vec2 a, const Vec2 b, float t) {
  if (t > 1) t = 1;
  if (t < 0) t = 0;
  return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

inline Vec2 Vec2::LerpUnclamped(const Vec2 a, const Vec2 b, float t) {
	return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}


inline float Vec2::DotProduct(Vec2 a, Vec2 b) {
	return a.x * b.x + a.y * b.y;
}

