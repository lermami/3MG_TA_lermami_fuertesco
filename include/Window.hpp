#ifndef __WINDOW_H__
#define __WINDOW_H__ 1

#include "GLFW/glfw3.h"
#include <optional>

class Window {
public:
  static std::optional<Window> create(int w, int h);
  bool is_done() const;
  void render();

  ~Window();
  Window(Window& w);
  Window(Window&& w) noexcept;
  Window(const Window&) = delete;

private:
  Window(int w, int h);
  GLFWwindow* handle_;
};

#endif