#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"

Camera::Camera(Window w, Vec3 pos, float speed, float sensitivity) {
  pos_ = pos;
  speed_ = speed;
  sensitivity_ = sensitivity;
}

Camera::~Camera() {

}

glm::mat4 Camera::getProjectionMatrix(float fov, float aspect, float near, float far) {
   return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 Camera::getProjectionMatrix(float left, float right, float bottom, float top, float near, float far) {
   return glm::ortho(left, right, bottom, top, near, far); 
}

glm::mat4 Camera::getViewMatrix(Vec3 target, Vec3 up) {
  glm::vec3 c = { pos_.x, pos_.y, pos_.z };
  glm::vec3 t = { target.x, target.y, target.z };
  glm::vec3 u = { up.x, up.y, up.z };

  return glm::lookAt(c, t, u);
}