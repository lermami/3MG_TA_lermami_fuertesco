#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.init(0.4, 0.4, 0.4, 1);

  if(glewInit() != GLEW_OK) return -1;

  Triangle t;

  std::string vertex_shader = ReadFiles("../include/test.vs");
  std::string fragment_shader = ReadFiles("../include/test.fs");

  // Shaders
  unsigned int vertexShader = CreateShader(0);
  CompileShader(vertexShader, vertex_shader.c_str());
  unsigned int fragmentShader = CreateShader(1);
  CompileShader(fragmentShader, fragment_shader.c_str());
  
  // Program
  unsigned int shaderProgram = CreateProgram(vertexShader, fragmentShader);

  // Buffers
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  InputMap inputMap(w);

  Input up(Key::kKey_W);
  Input left(Key::kKey_A);
  Input down(Key::kKey_S);
  Input right(Key::kKey_D);

  inputMap.addInput(&up);
  inputMap.addInput(&left);
  inputMap.addInput(&down);
  inputMap.addInput(&right);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);

    inputMap.updateInputs();
    //printf("\n%d", getchar());

    if (up.IsKeyPressed()) {
      t.move(0.0f, 1.0f * w.getDeltaTime(), 0.0f);
    }

    if (left.IsKeyPressed()) {
      t.move(-1.0f * w.getDeltaTime(), 0.0f, 0.0f);
    }

    if (down.IsKeyPressed()) {
      t.move(0.0f, -1.0f * w.getDeltaTime(), 0.0f);
    }

    if (right.IsKeyPressed()) {
      t.move(1.0f * w.getDeltaTime(), 0.0f, 0.0f);
    }

    // Draw triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(t), &t, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}