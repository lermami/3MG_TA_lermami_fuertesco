#pragma once
#include "GLFW/glfw3.h"
#include <optional>
#include <vector>
#include <ctime>

class Engine;

#define 	KEY_W   87
#define 	KEY_A   65
#define 	KEY_S   83
#define 	KEY_D   68

class Window {
public:
  friend class Input;

  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window");
  bool is_done() const;
  void swap();
  void clearColor(float r, float g, float b, float a) const;

  float getDeltaTime();
  void calculateCurrentTime();
  void calculateLastTime();

  ~Window();
  Window(Window& w);
  Window(Window&& w);
  Window(const Window&) = delete;

private:
  Window(int w, int h, const char* title);

  GLFWwindow* handle_;
  clock_t  currentTime_;
  clock_t  lastTime_;
  float deltaTime_;
};

