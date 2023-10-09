#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>
#include <fstream>

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"

unsigned int CreateShader(int type) {
  unsigned int shader = 0;

  if (type == 0) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == 1) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  }

  return shader;
}

void CompileShader(unsigned int id, const char* src) {
  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    printf("ERROR::SHADER::COMPILATION_FAILED\n");
  }
}

unsigned int CreateProgram(unsigned int vs, unsigned int fs) {
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);
  glLinkProgram(shaderProgram);

  // check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    return 0;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);

  return shaderProgram;
}

std::string ReadFiles(const std::string& file) {
  std::string final;
  std::ifstream fileStream(file, std::ios::in);

  if (!fileStream.is_open()) {
    printf("ERROR CAN'T READ FILE %s", file.c_str());
    return "ERROR";
  }

  std::string aux = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, aux);
    final.append(aux + "\n");
  }

  fileStream.close();
  return final;
}

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

  Input up(KEY_W);
  Input left(KEY_A);
  Input down(KEY_S);
  Input right(KEY_D);

  inputMap.addInput(&up);
  inputMap.addInput(&left);
  inputMap.addInput(&down);
  inputMap.addInput(&right);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);

    inputMap.updateInputs();

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