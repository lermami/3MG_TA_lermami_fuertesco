#include "camera.hpp"
#include "Window.hpp"
#include "input.hpp"
#include "Engine.hpp"
#include "default_components.hpp"
#include "component_manager.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "vector_2.hpp"


/*
Camera::Camera(Engine& e, TransformComponent tr, CameraComponent cam){
  auto& compM = e.getComponentManager();
  compM.add_entity(tr, cam);
}

Camera::~Camera() {

}

void Camera::setProjectionMode(ProjectionMode mode) {
  projectionMode_ = mode;
}

ProjectionMode Camera::getProjectionMode() {
  return projectionMode_;
}

glm::mat4 Camera::getPerspectiveMatrix(float fov, float aspect, float near, float far) {
   return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 Camera::getOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far) {
   return glm::ortho(left, right, bottom, top, near, far); 
}

glm::mat4 Camera::getViewMatrix(Vec3 target, Vec3 up) {
  glm::vec3 c = { pos_.x, pos_.y, pos_.z };
  glm::vec3 t = { target.x, target.y, target.z };
  glm::vec3 u = { up.x, up.y, up.z };

  return glm::lookAt(c, t, u);
}

void Camera::move(Vec3 vel) {

  if (vel.z < 0) {
    pos_ += forward_ * speed_;
  }

  if (vel.z > 0) {
    pos_ -= forward_ * speed_;
  }

  if (vel.x > 0) {
    pos_ += Vec3::CrossProduct(forward_, up_).Normalized();
  }

  if (vel.x < 0) {
    pos_ -= Vec3::CrossProduct(forward_, up_).Normalized();
  }

}

Vec3 Camera::getPosition() {
  return pos_;
}

Vec3 Camera::forward() {
  return forward_;
}

void Camera::updateForward(Input& input, const float w, const float h){
  static float alpha = -1.57f;
  static float omega = 0;
  static float last_alpha = -1.57f;
  static float last_omega = 0;
  static Vec2 first_pos(0.0f, 0.0f);

  double mouse_x, mouse_y;
  input.getMousePos(mouse_x, mouse_y);

  if (input.IsKeyDown(kKey_LeftClick)) {
    first_pos = Vec2((float)mouse_x, (float)mouse_y);
  }

  if (input.IsKeyPressed(kKey_LeftClick)) {
    alpha = last_alpha + ((float)mouse_x - first_pos.x) / w * sensitivity_;
    omega = last_omega + ((float)mouse_y - first_pos.y) / h * -1;
  }

  if (input.IsKeyUp(kKey_LeftClick)) {
    last_alpha = alpha;
    last_omega = omega;
  }

  forward_.x = cos(omega) * cos(alpha);
  forward_.y = sin(omega);
  forward_.z = cos(omega) * sin(alpha);
}

void Camera::doRender() {
  for (int i = 0; i < window_.getProgramListSize(); i++) {
    unsigned program = window_.getProgram(i);
    glUseProgram(program);
    switch (projectionMode_) {
      case ProjectionMode::kPerspective:
        glm::mat4 perspective = getPerspectiveMatrix(60.0f, 1024.0f / 768.0f, 0.01f, 100000.0f);

        glUniformMatrix4fv(glGetUniformLocation(program, "u_p_matrix"), 1, GL_FALSE, glm::value_ptr(perspective));
      break;
      case ProjectionMode::kOrthogonal:
        glm::mat4 ortographic = getOrthogonalMatrix(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100000.0f);

        glUniformMatrix4fv(glGetUniformLocation(program, "u_o_matrix"), 1, GL_FALSE, glm::value_ptr(ortographic));
      break;
    }

    //View
    glm::mat4 view = getViewMatrix(pos_ + forward_, up_);
    GLint viewMatrixLoc = glGetUniformLocation(program, "u_v_matrix");
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

    //Camera position
    GLint camPosLoc = glGetUniformLocation(program, "u_camera_pos");
    glUniform1fv(camPosLoc, sizeof(float) * 3, &pos_.x);
  }
}

*/
