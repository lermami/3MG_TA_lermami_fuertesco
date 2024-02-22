#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "camera.hpp"
#include "Input.hpp"
#include "component_manager.hpp"

int main(int, char**) {
  Engine e;
  auto& component_manager = e.getComponentManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window", false);
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  Geometry triangleGeo;

  triangleGeo.vertex_ = {
    {{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
    {{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
    {{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
  };

  triangleGeo.indices_ = { 0, 1, 2 };

  Vec3 tr_pos(0.0f, 0.0f, 0.0f);
  Vec3 tr_size(10.0f, 10.0f, 0.0f);
  Vec3 tr_rot(0.0f, 0.0f, 0.0f);

  size_t triangle = component_manager.add_entity();

  auto simpleProgram = CreateProgram(w, "../assets/raw_shader/raw.vs", "../assets/raw_shader/raw.fs");

  auto tr_render = component_manager.create_component<RenderComponent>(triangle);
  auto tr_transform = component_manager.create_component<TransformComponent>(triangle);
  init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
  init_render_component_system(*tr_render, "Triangle", triangleGeo, simpleProgram, NULL);

  //Camera
  size_t main_camera = component_manager.add_entity();
  auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
  w.setCurrentCam(main_camera);

  Input input_map(w);

  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();
    input_map.updateInputs();

    // Draw triangle
    w.render();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}