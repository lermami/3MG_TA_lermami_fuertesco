#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"
#include "thread_manager.hpp"
#include "buffer.hpp"

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4, 0.4, 0.4, 1);

  if(glewInit() != GLEW_OK) return -1;

  Triangle t("../include/test.vs", "../include/test.fs");

  Input inputMap(w);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);

    //Inputs
    inputMap.updateInputs();

    if (inputMap.IsKeyPressed('W')) {
      //t.move(0.0f, 1.0f * w.getDeltaTime(), 0.0f);
      t.addSize(0.01f, 0.01f);
    }

    if (inputMap.IsKeyPressed('A')) {
      //t.move(-1.0f * w.getDeltaTime(), 0.0f, 0.0f);
      t.roll(-0.01f);
    }

    if (inputMap.IsKeyPressed('S')) {
      //t.move(0.0f, -1.0f * w.getDeltaTime(), 0.0f);
      t.addSize(-0.01f, -0.01f);
    }

    if (inputMap.IsKeyPressed('D')) {
      //t.move(1.0f * w.getDeltaTime(), 0.0f, 0.0f);
      t.roll(0.01f);
    }

    double scroll_x = 0, scroll_y = 0;
    inputMap.getScroll(scroll_x, scroll_y);
    if (scroll_y > 0) {
      t.move(0.0f, 1.0f * w.getDeltaTime(), 0.0f);
    }
    if (scroll_y < 0) {
      t.move(0.0f, -1.0f * w.getDeltaTime(), 0.0f);
    }

    // Draw triangle
    t.render();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}