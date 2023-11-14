#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4, 0.4, 0.4, 1);

  if(glewInit() != GLEW_OK) return -1;

  Triangle t("../include/test.vs", "../include/test.fs");

  InputMap inputMap(w);

  Input up(inputMap, Key::kKey_W);
  Input left(inputMap, Key::kKey_A);
  Input down(inputMap, Key::kKey_S);
  Input right(inputMap, Key::kKey_D);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);

    //Inputs
    inputMap.updateInputs();

    if (up.IsKeyPressed()) {
      //t.move(0.0f, 1.0f * w.getDeltaTime(), 0.0f);
      t.addSize(0.01f, 0.01f);
    }

    if (left.IsKeyPressed()) {
      //t.move(-1.0f * w.getDeltaTime(), 0.0f, 0.0f);
      t.roll(-0.01f);
    }

    if (down.IsKeyPressed()) {
      //t.move(0.0f, -1.0f * w.getDeltaTime(), 0.0f);
      t.addSize(-0.01f, -0.01f);
    }

    if (right.IsKeyPressed()) {
      //t.move(1.0f * w.getDeltaTime(), 0.0f, 0.0f);
      t.roll(0.01f);
    }

    // Draw triangle
    t.render();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}