#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image/stbi_image.h"

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
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  std::vector<Vertex> triangle_mesh = {
    {-0.05f, -0.05f, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0.05f, -0.05f, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
    {0.0f, 0.05f, 0, 0, 0, 1, 1, 0, 0, 0, 0.5, 1},
  };

  std::vector<unsigned> tr_indices = { 0, 1, 2 };

  //Texture temp
  int width, height, nrChannels;
  unsigned char* laboon_tex_src = stbi_load("../assets/wall.jpg", &width, &height, &nrChannels, 0);

  unsigned int laboon_tex;
  glGenTextures(1, &laboon_tex);
  glBindTexture(GL_TEXTURE_2D, laboon_tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, laboon_tex_src);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(laboon_tex_src);

  Vec3 tr_pos(0.0f, 0.0f, 0.0f);
  Vec3 tr_size(10.0f, 10.0f, 0.0f);
  Vec3 tr_rot(0.0f, 0.0f, 0.0f);

  size_t triangle = component_manager.add_entity();

  auto simpleProgram = CreateProgram("../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");

  auto tr_render = component_manager.get_component<RenderComponent>(triangle);
  auto tr_transform = component_manager.get_component<TransformComponent>(triangle);
  init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
  init_vertex_system(*tr_render, triangle_mesh, tr_indices, simpleProgram, laboon_tex);


  while (!w.is_done()) {
    w.calculateLastTime();

    // Draw triangle
    render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}