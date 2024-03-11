#include "camera.hpp"
#include "Window.hpp"
#include "input.hpp"
#include "Engine.hpp"
#include "default_components.hpp"
#include "component_manager.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "vector_2.hpp"


CameraManager::CameraManager(Engine& e) : engine_{e} {

}

CameraManager::~CameraManager() {

}

void CameraManager::setProjectionMode(ProjectionMode mode) {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);
  cam_comp_.projectionMode_ = mode;
}

ProjectionMode CameraManager::getProjectionMode() {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);
  return cam_comp_.projectionMode_;
}

glm::mat4 CameraManager::getPerspectiveMatrix(float fov, float aspect, float near, float far) {
   return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 CameraManager::getOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far) {
   return glm::ortho(left, right, bottom, top, near, far); 
}

glm::mat4 CameraManager::getViewMatrix(Vec3 target, Vec3 up) {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);
  glm::vec3 c = { cam_comp_.pos_.x, cam_comp_.pos_.y, cam_comp_.pos_.z };
  glm::vec3 t = { target.x, target.y, target.z };
  glm::vec3 u = { up.x, up.y, up.z };

  return glm::lookAt(c, t, u);
}

void CameraManager::move(Vec3 vel) {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);

  if (vel.z < 0) {
    cam_comp_.pos_ += cam_comp_.forward_ * cam_comp_.speed_;
  }

  if (vel.z > 0) {
    cam_comp_.pos_ -= cam_comp_.forward_ * cam_comp_.speed_;
  }

  if (vel.x > 0) {
    cam_comp_.pos_ += Vec3::CrossProduct(cam_comp_.forward_, cam_comp_.up_).Normalized();
  }

  if (vel.x < 0) {
    cam_comp_.pos_ -= Vec3::CrossProduct(cam_comp_.forward_, cam_comp_.up_).Normalized();
  }

}

Vec3 CameraManager::getPosition() {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);
  return cam_comp_.pos_;
}

Vec3 CameraManager::forward() {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);
  return cam_comp_.forward_;
}

void CameraManager::updateForward(Input& input, const float w, const float h){
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);

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
    alpha = last_alpha + ((float)mouse_x - first_pos.x) / w * cam_comp_.sensitivity_;
    omega = last_omega + ((float)mouse_y - first_pos.y) / h * -1;
  }

  if (input.IsKeyUp(kKey_LeftClick)) {
    last_alpha = alpha;
    last_omega = omega;
  }

  cam_comp_.forward_.x = cos(omega) * cos(alpha);
  cam_comp_.forward_.y = sin(omega);
  cam_comp_.forward_.z = cos(omega) * sin(alpha);
}

void CameraManager::doRender(Window* w) {
  CameraComponent& cam_comp_ = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);

  for (int i = 0; i < w->getProgramListSize(); i++) {
    unsigned program = w->getProgram(i);
    glUseProgram(program);
    switch (cam_comp_.projectionMode_) {
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
    glm::mat4 view = getViewMatrix(cam_comp_.pos_ + cam_comp_.forward_, cam_comp_.up_);
    GLint viewMatrixLoc = glGetUniformLocation(program, "u_v_matrix");
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

    //CameraManager position
    GLint camPosLoc = glGetUniformLocation(program, "u_camera_pos");
    glUniform1fv(camPosLoc, sizeof(float) * 3, &cam_comp_.pos_.x);
  }
}

void CameraManager::setCurrentCam(size_t cam) {
  if (cam == 0) {
    auto& componentM = engine_.getComponentManager();
    auto cameraList = componentM.get_component_list<CameraComponent>();

    size_t camCount = 1;

    for (auto c = cameraList->begin(); c != cameraList->end(); c++) {
      if (!c->has_value()) continue;
      auto& camera = c->value();

      if (camera.name_ == "DefaultCamera") {
        current_cam_ = camCount;
      }
      camCount++;
    }
  }
  else {
    current_cam_ = cam;
  }
}

size_t CameraManager::getCurrentCam() {
  return current_cam_;
}

void CameraManager::mouseRotate(Input& input, const float w, const float h) {
  CameraComponent& cam = *engine_.getComponentManager().get_component<CameraComponent>(current_cam_);

  static float alpha = -1.57f;
  static float omega = 0;
  static Vec2 first_pos(0.0f, 0.0f);

  double mouse_x, mouse_y;
  input.getMousePos(mouse_x, mouse_y);

  if (input.IsKeyDown(kKey_RightClick)) {
    first_pos = Vec2((float)mouse_x, (float)mouse_y);
  }

  if (input.IsKeyPressed(kKey_RightClick)) {
    alpha = cam.last_alpha + ((float)mouse_x - first_pos.x) / w * cam.sensitivity_;
    omega = cam.last_omega + ((float)mouse_y - first_pos.y) / h * -1;
  }

  if (input.IsKeyUp(kKey_RightClick)) {
    cam.last_alpha = alpha;
    cam.last_omega = omega;
  }

  cam.forward_.x = cos(omega) * cos(alpha);
  cam.forward_.y = sin(omega);
  cam.forward_.z = cos(omega) * sin(alpha);
}