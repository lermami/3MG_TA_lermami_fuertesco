#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vector_3.hpp"

enum class ProjectionMode {
  kPerspective,
  kOrthogonal,
};

class Window;
class Input;

class Camera {
public:

  Camera(Window& w, Vec3 pos = Vec3(0.0f,0.0f,0.0f), float speed = 1.0f, float sensitivity = 1.0f);
  ~Camera();

  void setProjectionMode(ProjectionMode mode);
  ProjectionMode getProjectionMode();

  glm::mat4 getPerspectiveMatrix(float fov, float aspect, float near, float far);
  glm::mat4 getOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far);
  glm::mat4 getViewMatrix(Vec3 target, Vec3 up);

  void updateForward(Input& input, const float w, const float h);
  Vec3 forward();

  void move(Vec3 vel);
  Vec3 getPosition();

  void doRender();

private:
  Vec3 pos_;
  Vec3 forward_;
  Vec3 up_;
  Vec3 right_;

  float speed_;
  float sensitivity_;

  ProjectionMode projectionMode_;

  Window& window_;
};