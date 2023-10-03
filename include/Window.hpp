#pragma once
#include "GLFW/glfw3.h"
#include <optional>

class Window {
public:
  static std::optional<Window> create(int w, int h, const char* title = "Window");
  bool is_done() const;
  void render() const;
  void init(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const;

  ~Window();
  Window(Window& w);
  Window(Window&& w);
  Window(const Window&) = delete;

private:
  Window(int w, int h, const char* title);
  GLFWwindow* handle_;
};

