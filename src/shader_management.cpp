#include "shader_management.hpp"
#include "Window.hpp"

#include <GL/glew.h>
#include <fstream>

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

unsigned int CreateProgram(Window& w, const char* v, const char* f) {
  //Read shaders
  std::string vs = ReadFiles(v);
  std::string fs = ReadFiles(f);

  //Create opengl shaders
  auto vertexShader = CreateShader(0);
  CompileShader(vertexShader, vs.c_str());
  auto fragmentShader = CreateShader(1);
  CompileShader(fragmentShader, fs.c_str());

  //Attach and link shaders to program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  //Check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    return 0;
  }
  else {
    w.addProgram(shaderProgram);
  }

  //Clean shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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

void SetVector3(unsigned int program, char* name, Vec3 vector) {
  GLuint id = glGetUniformLocation(program, name);
  glUniform3f(id, vector.x, vector.y, vector.z);
}