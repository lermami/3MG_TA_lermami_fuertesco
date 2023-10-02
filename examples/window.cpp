#include <GLFW/glfw3.h>
#include <optional>
#include "Window.hpp"

int WinMain(int, char**) {

  glfwInit();

  auto maybe_w = Window::create(1024, 768);
  if (!maybe_w.has_value()) return -1;

  auto& w = maybe_w.value();
  while (!w.is_done()) {

  }


  glfwTerminate();
  return 0;
}