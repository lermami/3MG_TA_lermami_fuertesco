#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Engine.hpp"
#include "input.hpp"

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  Input input_map(w);

  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();

    input_map.updateInputs();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}