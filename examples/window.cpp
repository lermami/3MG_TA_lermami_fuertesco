#include <GLFW/glfw3.h>
#include <optional>
#include "Window.hpp"
#include "Engine.hpp"

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.init(1, 0, 1, 1);

  while (!w.is_done()) {

    w.swap();
  }

  return 0;
}