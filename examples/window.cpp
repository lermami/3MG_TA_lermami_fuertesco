#include <GLFW/glfw3.h>
#include <optional>
#include "Window.hpp"
#include "Engine.hpp"

int WinMain(int, char**) {

  Engine::init();
  
  auto maybe_w = Window::create(1024, 768, "Test Window");
  if (!maybe_w.has_value()) return -1;

  auto& w = maybe_w.value();
  w.init(1, 0, 1, 1);

  while (!w.is_done()) {
    w.render();
  }

  Engine::terminate();

  return 0;
}