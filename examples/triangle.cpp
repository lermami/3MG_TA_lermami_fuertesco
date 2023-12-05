#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "component_manager.hpp"

int main(int, char**) {
  Engine e;
  ComponentManager component_manager;

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4, 0.4, 0.4, 1);

  if (glewInit() != GLEW_OK) return -1;

  std::vector<Vertex> triangle_mesh = {
    {-0.05f, -0.05f, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0.05f, -0.05f, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    {0.0f, 0.05f, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  };

  std::vector<unsigned> tr_indices = { 0, 1, 2 };

  Vec3 tr_pos(0.0f, 0.0f, 0.0f);
  Vec3 tr_size(10.0f, 10.0f, 0.0f);
  Vec3 tr_rot(0.0f, 0.0f, 0.0f);

  size_t triangle = component_manager.add_entity();

  auto simpleProgram = CreateProgram("../assets/test_shader/test.vs", "../assets/test_shader/test.fs");

  auto tr_render = component_manager.get_component<RenderComponent>(triangle);
  auto tr_transform = component_manager.get_component<TransformComponent>(triangle);
  init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
  init_vertex_system(*tr_render, triangle_mesh, tr_indices, simpleProgram);


  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);


    // Draw triangle
    render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}