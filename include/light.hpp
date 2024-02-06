#pragma once

#include "vector_3.hpp"
#include "enum.hpp"

struct LightComponent{
public:

  LightComponent();

  //Ambient
  LightComponent(Vec3 color, Vec3 specular);

  //Directional
  LightComponent(Vec3 direction, Vec3 color, Vec3 specular);

  //Point
  LightComponent(Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic);

  //Spot
  LightComponent(Vec3 direction, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle);


  ~LightComponent();

  LightType type_;

  Vec3 pos_;
  Vec3 color_;
  Vec3 spec_color_;

  Vec3 direction_;

  float constant_;
  float linear_;
  float quadratic_;

  float cutoff_angle_;
};