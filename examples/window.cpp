#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "input.hpp"

//In this example we will see how to create an open a window
int main(int, char**) {
  //[1]. Set engine variable
  Engine e;
  
  //[2]. Create a window variable
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  //[3]. Take the window reference
  auto& w = maybe_w.value();

  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  Input input_map(w);

  //[4]. Create main loop
  while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
    w.calculateLastTime();

    input_map.updateInputs();

    //[5].Swap buffer to update window info
    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}