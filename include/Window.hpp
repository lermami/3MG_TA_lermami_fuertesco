#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <optional>
#include <vector>
#include <ctime>
#include "enum.hpp"
#include "glm/glm.hpp"

class Engine;

class Window {
public:
  friend class Input;
  friend class Camera;

  ~Window();
  Window(Window& w);
  Window(Window&& w) noexcept;
  Window(const Window&) = delete;

  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window");
  bool is_done() const;
  void swap();
  void clearColor(float r, float g, float b, float a) const;

  void initImGui();
  void updateImGui();

  void initSoundContext();

  float getDeltaTime();
  void calculateCurrentTime();
  void calculateLastTime();

  void enableCulling(bool enable);
  void setCullingMode(CullingMode culling, FrontFace frontface);
  void enableDepthTest(bool enable);
  void setDepthTestMode(DepthTestMode mode);

  void getWindowSize(unsigned int& w, unsigned int& h);
  void setwindowsize(unsigned int w, unsigned int h);

  void addProgram(unsigned new_program);
  unsigned getProgram(int n);
  int getProgramListSize();

  void renderLights();
  void render(unsigned int depth_map);
  void renderShadowMap(unsigned int depth_map, unsigned int program);

  void setCurrentCam(size_t cam);
  size_t getCurrentCam();

  glm::mat4 ConfigureShaderAndMatrices();

private:
  Window(Engine& e, int w, int h, const char* title);
  void renderImgui();

  GLFWwindow* handle_;
  clock_t  currentTime_;
  clock_t  lastTime_;
  unsigned int width_;
  unsigned int height_;
  float deltaTime_;
  bool imguiInit_;

  size_t current_cam_;

  std::vector<unsigned> program_list_;
  Engine& engine_;
};

