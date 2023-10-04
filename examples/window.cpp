#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "Window.hpp"
#include "Engine.hpp"

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

void CompileShader(unsigned int id, char* src) {
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

int main(int, char**) {
  Engine e;
  
  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.init(0.4, 0.4, 0.4, 1);

  if(glewInit() != GLEW_OK) return -1;

  class Vec3 {
  public:
    Vec3() {
      x_ = 0;
      y_ = 0;
      z_ = 0;
    }
    void translate(float x, float y, float z) {
      x_ += x;
      y_ += y;
      z_ += z;
    }
  private:
    float x_, y_, z_;
  };

  struct Vertex {
    Vec3 position_;
    Vec3 colors_;
    Vec3 normal_;
  };

  struct Triangle {
    void update();
    Vertex vertex_[3];
  };

  Triangle t;
  t.vertex_[0].position_.translate(-0.5, -0.5, 0);
  t.vertex_[1].position_.translate(0.5, -0.5, 0);
  t.vertex_[2].position_.translate(0, 0.5, 0);

  t.vertex_[0].colors_.translate(1, 0, 0);
  t.vertex_[1].colors_.translate(0, 1, 0);
  t.vertex_[2].colors_.translate(0, 0, 1);

  char* vertex_shader =
    "#version 400\n"
    "layout(location = 0) in vec3 vp;"
    "layout(location = 1) in vec3 a_color;"
    "out vec3 color;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
    "  color = a_color;"
    "}";

  char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "in vec3 color;"
    "void main() {"
    "  frag_colour = vec4(color, 1.0);"
    "}";

  // Shaders
  unsigned int vertexShader = CreateShader(0);
  CompileShader(vertexShader, vertex_shader);
  unsigned int fragmentShader = CreateShader(1);
  CompileShader(fragmentShader, fragment_shader);
  
  // Program
  unsigned int shaderProgram = CreateProgram(vertexShader, fragmentShader);

  // Buffers
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(t), &t.vertex_[0].position_, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  /*
  struct KeyMap {
    Window::Input up(KEY_W);
    Window::Input down(KEY_S);
    Window::Input left(KEY_A);
    Window::Input right(KEY_D);
  };
  */

  while (!w.is_done()) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    w.swap();
  }

  return 0;
}