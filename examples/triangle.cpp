#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "camera.hpp"
#include "Input.hpp"
#include "component_manager.hpp"

int main(int, char**) {
  Engine e;
  auto& component_manager = e.getComponentManager();
  auto& resourceM = e.getResourceManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window", false);
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  Renderer renderer(e, w);

  Geometry triangleGeo;

  triangleGeo.vertex_ = {
    {{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
    {{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
    {{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
  };

  triangleGeo.indices_ = { 0, 1, 2 };

  resourceM.createBuffersWithGeometry(&triangleGeo, "TriangleVertices", "TriangleIndices");
  auto simpleProgram = CreateProgram(w, "../assets/raw_shader/raw.vs", "../assets/raw_shader/raw.fs");

  size_t triangle = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 0.0f)),
    RenderComponent("Triangle", "TriangleVertices", "TriangleIndices", simpleProgram, 0));

  //Camera
  size_t main_camera = component_manager.add_entity(CameraComponent());
  w.setCurrentCam(main_camera);

  Input input_map(w);

  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();
    input_map.updateInputs();

    // Draw triangle
    renderer.render();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}