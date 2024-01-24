#pragma once

#include "vector_3.hpp"
#include <glm/glm.hpp>

enum class ProjectionMode {
  kPerspective,
  kOrthogonal,
};

class Window;

class Camera {
public:

  Camera(Window w, Vec3 pos = Vec3(0.0f,0.0f,0.0f), float speed = 1.0f, float sensitivity = 1.0f);
  ~Camera();

  glm::mat4 getProjectionMatrix(float fov, float aspect, float near, float far);
  glm::mat4 getProjectionMatrix(float left, float right, float bottom, float top, float near, float far);
  glm::mat4 getViewMatrix(Vec3 target, Vec3 up);

private:
  Vec3 pos_;
  float speed_;
  float sensitivity_;
};