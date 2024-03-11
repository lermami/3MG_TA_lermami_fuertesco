#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vector_3.hpp"
#include "enum.hpp"

class Window;
class Engine;
class Input;
struct TransformComponent;
struct CameraComponent;

class CameraManager {
public:

  CameraManager(Engine& e);
  ~CameraManager();

  void setProjectionMode(ProjectionMode mode);
  ProjectionMode getProjectionMode();

  glm::mat4 getPerspectiveMatrix(float fov, float aspect, float near, float far);
  glm::mat4 getOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far);
  glm::mat4 getViewMatrix(Vec3 target, Vec3 up);

  void updateForward(Input& input, const float w, const float h);
  Vec3 forward();

  void move(Vec3 vel);
  Vec3 getPosition();

  void doRender(Window* w);

  void setCurrentCam(size_t cam);
  size_t getCurrentCam();

  void mouseRotate(Input& input, const float w, const float h);
private:
  Engine& engine_;
  size_t current_cam_;
  size_t default_cam_;
};
