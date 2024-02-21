#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "component_manager.hpp"
#include "camera.hpp"

#include "sound/soundbuffer.h"

int main(int, char**) {
  Engine e;
  auto& component_manager = e.getComponentManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  w.initImGui();
  w.initSoundContext();

  Geometry triangleGeo;

  triangleGeo.vertex_ = {
    {{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
    {{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
    {{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
  };

  triangleGeo.indices_= { 0, 1, 2 };

  Vec3 tr_pos(-0.5f, 0.0f, 0.0f);
  Vec3 tr_size(10.0f, 10.0f, 0.0f);
  Vec3 tr_rot(0.0f, 0.0f, 0.0f);

  auto simpleProgram = CreateProgram(w, "../assets/raw_shader/raw.vs", "../assets/raw_shader/raw.fs");

  size_t triangle = component_manager.add_entity();

  auto tr_render = component_manager.create_component<RenderComponent>(triangle);
  auto tr_transform = component_manager.create_component<TransformComponent>(triangle);
  auto tr_audio = component_manager.create_component<AudioComponent>(triangle);

  init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
  init_render_component_system(*tr_render, triangleGeo, simpleProgram, NULL);

  ALfloat pos[3] = { 0,0,0 };
  ALfloat vel[3] = { 0,0,0 };

  SoundBuffer b = SoundBuffer::MakeBuffer("../assets/test.wav").value();
  init_audio_system(*tr_audio, b, "Test1", pos, vel, 1.0f, 1.0f, true);

  size_t triangle2 = component_manager.add_entity();
  Vec3 tr_pos2(0.5f, 0.0f, 0.0f);

  auto tr_render2 = component_manager.create_component<RenderComponent>(triangle2);
  auto tr_transform2 = component_manager.create_component<TransformComponent>(triangle2);
  auto tr_audio2 = component_manager.create_component<AudioComponent>(triangle2);

  init_transform_system(*tr_transform2, tr_pos2, tr_rot, tr_size);
  init_render_component_system(*tr_render2, triangleGeo, simpleProgram, NULL);

  SoundBuffer b2 = SoundBuffer::MakeBuffer("../assets/test2.wav").value();
  init_audio_system(*tr_audio2, b2, "Test2", pos, vel, 1.0f, 1.0f, false);

  //Camera
  size_t main_camera = component_manager.add_entity();
  auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
  w.setCurrentCam(main_camera);

  while (!w.is_done()) {
    w.calculateLastTime();
    
    w.updateImGui();

    basic_sound_system(*component_manager.get_component_list<AudioComponent>());

    // Draw triangle
    imgui_transform_system(*component_manager.get_component_list<TransformComponent>());

    w.render();
    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}