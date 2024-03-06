#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vector_3.hpp"

class Window;
class Engine;
class Input;
struct TransformComponent;
struct CameraComponent;

class Camera {
public:

  /*
  Camera(Engine& e, TransformComponent tr, CameraComponent cam);
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
  */

private:
};
