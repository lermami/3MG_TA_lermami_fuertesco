#include "shader_management.hpp"
#include "Window.hpp"

#include <GL/glew.h>
#include <fstream>

unsigned int Shader::CreateShader(int type) {
  unsigned int shader = 0;

  if (type == 0) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == 1) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  }

  return shader;
}

void Shader::CompileShader(unsigned int id, const char* src) {
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

std::optional<Shader> Shader::create(Window& w, const char* v, const char* f) {
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
    return std::nullopt;
  }
  else {
    //Clean shaders
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Shader result(shaderProgram);
    w.addProgram(std::move(result));
    return result;
  }

}

std::string Shader::ReadFiles(const std::string& file) {
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

void Shader::SetVector3(char* name, Vec3 vector) {
  GLuint id = glGetUniformLocation(handle_, name);
  glUniform3f(id, vector.x, vector.y, vector.z);
}

Shader::Shader(){
  handle_ = 0;
  destroy_ = true;
}

Shader::Shader(unsigned handle) : handle_{handle}{
  destroy_ = true;
}

Shader::~Shader(){
  if (destroy_) {
    glDeleteProgram(handle_);
  }
}

Shader::Shader(Shader&& o) : handle_{ o.handle_ }, destroy_{ o.destroy_ } {
  o.destroy_ = false;
}

Shader& Shader::operator=(Shader&& o) {
  handle_ = o.handle_;
  destroy_ = o.destroy_;
  o.destroy_ = false;
  return *this;
}

unsigned Shader::get() {
  return handle_;
}