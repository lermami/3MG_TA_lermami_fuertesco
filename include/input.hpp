#pragma once

#include "GLFW/glfw3.h"
#include <vector>

class Window;
class Input;

class Key {
  friend class Input;
public:

  ~Key() = default;

  void setState(unsigned int state);
  unsigned int getState() const;

  void setKey(int key);
  int getKey() const;
private:

  Key();
  unsigned int state_;
  int key_;
};

class Input {
public:
  Input(Window& w);
  ~Input();

  bool IsKeyDown(char key);
  bool IsKeyPressed(char key);
  bool IsKeyUp(char key);
  bool IsKeyDown(int key);
  bool IsKeyPressed(int key);
  bool IsKeyUp(int key);

  void updateInputs();

  void setScroll(double scroll_x, double scroll_y);
  void getScroll(double& scroll_x, double& scroll_y);

  void getMousePos(double &x, double &y);

private:
  GLFWwindow* windowHandle_;
  Key input_[88];

  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  inline static double scroll_x_;
  inline static double scroll_y_;

  double mouse_x_;
  double mouse_y_;
};

