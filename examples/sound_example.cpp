#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "component_manager.hpp"

#include "sound/soundbuffer.h"

int main(int, char**) {
  Engine e;
  ComponentManager component_manager;

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  if (glewInit() != GLEW_OK) return -1;

  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* ctx = alcCreateContext(device, NULL);
  alcMakeContextCurrent(ctx);

  w.initImGui();

  std::vector<Vertex> triangle_mesh = {
    {-0.05f, -0.05f, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0.05f, -0.05f, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    {0.0f, 0.05f, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  };

  std::vector<unsigned> tr_indices = { 0, 1, 2 };

  Vec3 tr_pos(-0.5f, 0.0f, 0.0f);
  Vec3 tr_size(10.0f, 10.0f, 0.0f);
  Vec3 tr_rot(0.0f, 0.0f, 0.0f);

  auto simpleProgram = CreateProgram("../assets/test_shader/test.vs", "../assets/test_shader/test.fs");


  size_t triangle = component_manager.add_entity();

  auto tr_render = component_manager.get_component<RenderComponent>(triangle);
  auto tr_transform = component_manager.get_component<TransformComponent>(triangle);
  auto tr_audio = component_manager.get_component<AudioComponent>(triangle);


  init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
  init_vertex_system(*tr_render, triangle_mesh, tr_indices, simpleProgram);

  ALfloat pos[3] = { 0,0,0 };
  ALfloat vel[3] = { 0,0,0 };

  SoundBuffer b = SoundBuffer::MakeBuffer("../assets/test.wav").value();
  init_audio_system(*tr_audio, b, "Test1", pos, vel, 1.0f, 1.0f, true);

  size_t triangle2 = component_manager.add_entity();
  Vec3 tr_pos2(0.5f, 0.0f, 0.0f);

  auto tr_render2 = component_manager.get_component<RenderComponent>(triangle2);
  auto tr_transform2 = component_manager.get_component<TransformComponent>(triangle2);
  auto tr_audio2 = component_manager.get_component<AudioComponent>(triangle2);

  init_transform_system(*tr_transform2, tr_pos2, tr_rot, tr_size);
  init_vertex_system(*tr_render2, triangle_mesh, tr_indices, simpleProgram);

  SoundBuffer b2 = SoundBuffer::MakeBuffer("../assets/test2.wav").value();
  init_audio_system(*tr_audio2, b2, "Test2", pos, vel, 1.0f, 1.0f, false);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);
    
    w.updateImGui();

    basic_sound_system(*component_manager.get_component_list<AudioComponent>());

    // Draw triangle
    render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}