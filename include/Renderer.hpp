#pragma once

#include "glm/glm.hpp"

class Engine;
class Window;

class Renderer {
public:
  Renderer(Engine& e, Window& w);
  ~Renderer();

  void renderLights();
  void render();
  void renderShadowMap(unsigned int program);

  glm::mat4 ConfigureShaderAndMatrices();

private:
  void renderImgui();

  Engine& engine_;
  Window& window_;

  bool renderShadows_;
  unsigned depthmap_;
  unsigned depthmapFBO_;
  unsigned shadowProgram_;
};