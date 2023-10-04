#pragma once
#include "GLFW/glfw3.h"
#include <optional>

class Engine;

#define 	KEY_W   87
#define 	KEY_A   65
#define 	KEY_S   83
#define 	KEY_D   68


class Window {
public:
  class Input {
  public:

    Input(int key);
    ~Input() = default;
    void update(Window& w);

    bool IsKeyDown();
    bool IsKeyPressed();
    bool IsKeyUp();
  private:
    /*
    0 -> Inactive
    1 -> Key down
    2 -> Key pressed
    3 -> Key up
    */
    unsigned int state_;
    int key_;
  };
  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window");
  bool is_done() const;
  void swap() const;
  void init(float r, float g, float b, float a) const;

  ~Window();
  Window(Window& w);
  Window(Window&& w);
  Window(const Window&) = delete;

private:
  Window(int w, int h, const char* title);
  GLFWwindow* handle_;
};

