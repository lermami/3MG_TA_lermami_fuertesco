#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>

#include "Window.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "default_systems.hpp"
#include "input.hpp"
#include "component_manager.hpp"
#include "camera.hpp"

//In this example we will see how to use input in the engine

int main(int, char**) {
  Engine e;
  auto& component_manager = e.getComponentManager();
  auto& resourceM = e.getResourceManager();

  auto& cameraM = e.getCameraManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
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

  auto simpleProgram = CreateProgram(w, "../assets/BasicShader/BasicColor/VertexColor.vs", "../assets/BasicShader/BasicColor/VertexColor.fs");
  resourceM.createBuffersWithGeometry(&triangleGeo, "TriangleVertices", "TriangleIndices");

  size_t triangle = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 0.0f)),
    RenderComponent("Triangle", "TriangleVertices", "TriangleIndices", simpleProgram, 0));

  //[1]. Create a input variable and link it to the window you are using
  Input input_map(w);

  size_t main_camera = component_manager.add_entity(CameraComponent());

  //(Special keys always need enum like "kKey_Escape")
  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();

    //[2]. Update your inputs at the begin of the main loop
    //Inputs
    input_map.updateInputs();

    float input_x = 0, input_y = 0;
    float rotate = 0;
    float input_velocity = 1.0f * w.getDeltaTime();

    double mouse_x = 0, mouse_y = 0;
    double x_scroll = 0, y_scroll = 0;

    //(To get Special inputs like mouse inputs it is necessary to use auxiliar variables)
    input_map.getMousePos(mouse_x, mouse_y);
    input_map.getScroll(x_scroll, y_scroll);

    //(Detect key input example) (With letters sintaxis could be 'W' or use enum "kKey_W")
    if (input_map.IsKeyPressed('W')) {
      input_y = input_velocity;
    }

    if (input_map.IsKeyPressed('S')) {
      input_y = -input_velocity;
    }
    if (input_map.IsKeyPressed('A')) {
      input_x = -input_velocity;
    }

    if (input_map.IsKeyPressed('D')) {
      input_x = input_velocity;
    }


    //(Detect scroll input example)
    if (y_scroll > 0) {
      rotate = -input_velocity * 2.0f;
    }

    if (y_scroll < 0) {
      rotate = input_velocity * 2.0f;
    }

    // Draw triangle
    move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
    rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, 0.0f, rotate));

    renderer.render();

    w.swap();
    w.calculateCurrentTime();
  }

  return 0;
}