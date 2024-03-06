#pragma once

#include "vector_3.hpp"
#include "enum.hpp"

struct LightComponent{
public:

  LightComponent();

  //Ambient
  LightComponent(Vec3 color);

  //Directional
  LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);

  //Point
  LightComponent(Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);

  //Spot
  LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);


  ~LightComponent();

  LightType type_;

  Vec3 color_;
  Vec3 spec_color_;

  Vec3 direction_;

  float constant_;
  float linear_;
  float quadratic_;

  float cutoff_angle_;

  float min_shadow_render_distance_;
  float max_shadow_render_distance_;
};