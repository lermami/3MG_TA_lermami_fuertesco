#pragma once

#include "math_library/vector_3.hpp"

enum class LigthType {
  kDirectional,
  kPoint,
  kSpot,
  kAmbient,
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float cutoff_angle;

  vec3 result;
};

struct PointLight {
  vec3 position;

  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  vec3 result;
};

class Ligth{
public:
  Ligth(Vec3 direction, Vec3 color, Vec3 specular);
  Ligth(Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic);
  ~Ligth();

  void setColor();
  Vec3 getColor();

private:
  Vec3 pos_;
  Vec3 color_;
  Vec3 spec_color_;

  Vec3 direction_;

  float constant_;
  float linear_;
  float quadratic_;

  float cutoff_angle_;
};