#include "light.hpp"

LightComponent::LightComponent() {
  direction_ = Vec3{ 0.0f,0.0f,0.0f };
  pos_ = Vec3{ 0.0f,0.0f,0.0f };
  color_ = Vec3{ 0.0f,0.0f,0.0f };
  spec_color_ = 0.0f;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  target_ = LightType::kAmbient;
}

LightComponent::LightComponent(Vec3 color, Vec3 specular) {
  direction_ = Vec3{0.0f,0.0f,0.0f};
  pos_ = Vec3{0.0f,0.0f,0.0f};
  color_ = color;
  spec_color_ = specular;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  target_ = LightType::kAmbient;
}

LightComponent::LightComponent(Vec3 direction, Vec3 color, Vec3 specular) {
  direction_ = direction;
  pos_ = Vec3{ 0.0f,0.0f,0.0f };
  color_ = color;
  spec_color_ = specular;
  constant_ = 0.0f;
  linear_ = 0.0f;
  quadratic_ = 0.0f;
  cutoff_angle_ = 0.0f;

  target_ = LightType::kDirectional;
}

LightComponent::LightComponent(Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic) {
  direction_ = Vec3{ 0.0f,0.0f,0.0f };
  pos_ = position;
  color_ = color;
  spec_color_ = specular;
  constant_ = constant;
  linear_ = linear;
  quadratic_ = quadratic;
  cutoff_angle_ = 0.0f;

  target_ = LightType::kPoint;
}

LightComponent::LightComponent(Vec3 direction, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle) {
  direction_ = direction;
  pos_ = position;
  color_ = color;
  spec_color_ = specular;
  constant_ = constant;
  linear_ = linear;
  quadratic_ = quadratic;
  cutoff_angle_ = cutoff_angle;

  target_ = LightType::kSpot;
}

LightComponent::~LightComponent() {

}