#include "light.hpp"

LightComponent::LightComponent() {
  spec_color_ = 0.0f;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  max_shadow_render_distance_ = 0.0f;
  min_shadow_render_distance_ = 0.0f;

  type_ = LightType::kAmbient;
}

LightComponent::LightComponent(Vec3 color) {
  color_ = color;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  max_shadow_render_distance_ = 0.0f;
  min_shadow_render_distance_ = 0.0f;

  type_ = LightType::kAmbient;
}

LightComponent::LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float min_shadow_render_distance, float max_shadow_render_distance) {
  direction_ = direction;
  color_ = color;
  spec_color_ = specular;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  max_shadow_render_distance_ = max_shadow_render_distance;
  min_shadow_render_distance_ = min_shadow_render_distance;

  type_ = LightType::kDirectional;
}

LightComponent::LightComponent(Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float min_shadow_render_distance, float max_shadow_render_distance) {
  direction_ = Vec3{ 0.0f,0.0f,0.0f };
  color_ = color;
  spec_color_ = specular;
  constant_ = constant;
  linear_ = linear;
  quadratic_ = quadratic;
  cutoff_angle_ = 0.0f;

  max_shadow_render_distance_ = max_shadow_render_distance;
  min_shadow_render_distance_ = min_shadow_render_distance;

  type_ = LightType::kPoint;
}

LightComponent::LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle, float min_shadow_render_distance, float max_shadow_render_distance) {
  direction_ = direction;
  color_ = color;
  spec_color_ = specular;
  constant_ = constant;
  linear_ = linear;
  quadratic_ = quadratic;
  cutoff_angle_ = cutoff_angle;

  max_shadow_render_distance_ = max_shadow_render_distance;
  min_shadow_render_distance_ = min_shadow_render_distance;

  type_ = LightType::kSpot;
}

LightComponent::~LightComponent() {

}