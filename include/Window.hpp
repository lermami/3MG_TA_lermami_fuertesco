#pragma once
#include "GLFW/glfw3.h"
#include <optional>
#include <vector>
#include <ctime>

class Engine;

class Window {
public:
  friend class Input;

  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window");
  bool is_done() const;
  void swap();
  void clearColor(float r, float g, float b, float a) const;

  void initImGui();
  void updateImGui();

  float getDeltaTime();
  void calculateCurrentTime();
  void calculateLastTime();

  ~Window();
  Window(Window& w);
  Window(Window&& w);
  Window(const Window&) = delete;

private:
  Window(int w, int h, const char* title);
  void renderImgui();

  GLFWwindow* handle_;
  clock_t  currentTime_;
  clock_t  lastTime_;
  float deltaTime_;
  bool imguiInit_;
};

