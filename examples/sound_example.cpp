#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Engine.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(int, char**) {
  Engine e;

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  if (glewInit() != GLEW_OK) return -1;

  w.initImGui();

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);
    w.updateImGui();


    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}