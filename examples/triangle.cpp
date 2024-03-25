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


//In this example we will see how to introduce a custom geometry
int main(int, char**) {

  //[1]. Init the engine
  Engine e;

  //[2]. Get component and resource manager
  auto& component_manager = e.getComponentManager();
  auto& resourceM = e.getResourceManager();

  auto maybe_w = Window::create(e, 1024, 768, "Test Window", false);
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  Renderer renderer(e, w);

  //[3]. Create and init the geometry it could be 2D like this example or 3D (Values are on screen coordinates e.g. -1 t +1)
  Geometry triangleGeo;
  triangleGeo.vertex_ = {
    {{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
    {{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
    {{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
  };

  //[4]. Set the index of the triangles that compose the geometry
  triangleGeo.indices_ = { 0, 1, 2 };

  //[5]. Create a buffer to store the geometry
  resourceM.createBuffersWithGeometry(&triangleGeo, "TriangleVertices", "TriangleIndices");

  //[6].  Create a program for the geometry (This Shader is one of the basic shaders present on the engine) this shaders uses the color of the vertex
  auto simpleProgram = CreateProgram(w, "../assets/BasicShader/BasicColor/VertexColor.vs", "../assets/BasicShader/BasicColor/VertexColor.fs");

  //[7]. Add an entity that use the geometry you have created with your program
  size_t triangle = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 0.0f)),
    RenderComponent("Triangle", "TriangleVertices", "TriangleIndices", simpleProgram, 0));

  Input input_map(w);

  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();
    input_map.updateInputs();

    //[8]. Render geometry
    renderer.render();

    w.swap();
  }

  return 0;
}