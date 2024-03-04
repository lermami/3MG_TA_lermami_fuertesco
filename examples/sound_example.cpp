#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "shader_management.hpp"
#include "Renderer.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "component_manager.hpp"
#include "Input.hpp"
#include "camera.hpp"

#include "sound/soundbuffer.h"

int main(int, char**) {
  Engine e;
  auto& component_manager = e.getComponentManager();
  auto& resourceM = e.getResourceManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window", true);
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  w.initSoundContext();

  Renderer renderer(e, w);

  Geometry triangleGeo;

  triangleGeo.vertex_ = {
    {{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
    {{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
    {{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
  };

  triangleGeo.indices_= { 0, 1, 2 };

  auto simpleProgram = CreateProgram(w, "../assets/raw_shader/raw.vs", "../assets/raw_shader/raw.fs");
  resourceM.createBuffersWithGeometry(triangleGeo, "TriangleVertices", "TriangleIndices");

  SoundBuffer testBuffer = SoundBuffer::MakeBuffer("../assets/test.wav").value();
  ALfloat pos[3] = { 0,0,0 };
  ALfloat vel[3] = { 0,0,0 };

  size_t triangle = component_manager.add_entity(TransformComponent(Vec3(-0.5f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 0.0f)),
    RenderComponent("Triangle 1", "TriangleVertices", "TriangleIndices", simpleProgram, 0),
    AudioComponent(testBuffer, "Test1", pos, vel, 1.0f, 1.0f, true));

  SoundBuffer testBuffer2 = SoundBuffer::MakeBuffer("../assets/test2.wav").value();
  size_t triangle2 = component_manager.add_entity(TransformComponent(Vec3(0.5f, 0.0f, 0.0), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 0.0f)),
    RenderComponent("Triangle 2", "TriangleVertices", "TriangleIndices", simpleProgram, 0),
    AudioComponent(testBuffer2, "Test2", pos, vel, 1.0f, 1.0f, false));

  //Camera
  size_t main_camera = component_manager.add_entity(CameraComponent());
  w.setCurrentCam(main_camera);

  Input input_map(w);

  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();
    input_map.updateInputs();
    w.updateImGui();

    basic_sound_system(*component_manager.get_component_list<AudioComponent>());

    // Draw triangle
    imgui_transform_system(e, w);

    renderer.render();
    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}