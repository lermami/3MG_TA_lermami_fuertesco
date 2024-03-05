#pragma once

#include "glm/glm.hpp"

class Engine;
class Window;
class Vec3;

class Renderer {
public:
  Renderer(Engine& e, Window& w);
  ~Renderer();

  void renderLights();
  void CalculateShadowsMatrix();
  void render();
  void renderShadowMap(unsigned int program);

  glm::mat4 ConfigureShadowMatrix(float near, float far, Vec3 pos, Vec3 direction);

private:
  void renderImgui();

  Engine& engine_;
  Window& window_;

  bool renderShadows_;
  unsigned int shadow_resolution_;
  unsigned depthmap_;
  unsigned depthmapFBO_;
  unsigned shadowProgram_;
};