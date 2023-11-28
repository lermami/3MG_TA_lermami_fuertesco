#pragma once

#include <vector>

enum Key {
  kKey_LeftClick = 0,
  kKey_RightClick,
  kKey_MiddleClick,
  kKey_AditionalMouseButton1,
  kKey_AditionalMouseButton2,
  kKey_AditionalMouseButton3,
  kKey_AditionalMouseButton4,
  kKey_Space = 32,
  kKey_Aphostrophe = 39,
  kKey_Coma = 44,
  kKey_Dot,
  kKey_Slash = 47,
  kKey_0,
  kKey_1,
  kKey_2,
  kKey_3,
  kKey_4,
  kKey_5,
  kKey_6,
  kKey_7,
  kKey_8,
  kKey_9,
  kKey_A = 65,
  kKey_B,
  kKey_C,
  kKey_D,
  kKey_E,
  kKey_F,
  kKey_G,
  kKey_H,
  kKey_I,
  kKey_J,
  kKey_K,
  kKey_L,
  kKey_M,
  kKey_N,
  kKey_O,
  kKey_P,
  kKey_Q,
  kKey_R,
  kKey_S,
  kKey_T,
  kKey_U,
  kKey_V,
  kKey_W,
  kKey_X,
  kKey_Y,
  kKey_Z,
  kKey_LeftBracket = 91,
  kKey_RightBracket = 93,
  kKey_Escape = 256,
  kKey_Enter,
  kKey_Tab,
  kKey_BackSpace,
  kKey_Insert,
  kKey_Delete,
  kKey_Right,
  kKey_Left,
  kKey_Down,
  kKey_Up,
  kKey_PageUp,
  kKey_PageDown,
  kKey_Start,
  kKey_End,
  kKey_CapsLock = 280,
  kKey_ScrollLock,
  kKey_NumLock,
  kKey_Pause = 284,
  kKey_F1 = 290,
  kKey_F2,
  kKey_F3,
  kKey_F4,
  kKey_F5,
  kKey_F6,
  kKey_F7,
  kKey_F8,
  kKey_F9,
  kKey_F10,
  kKey_F11,
  kKey_F12,
  kKey_LeftShift = 340,
  kKey_LeftControl,
  kKey_LeftAlt,
  kKey_LeftSuper,
  kKey_RightShift,
  kKey_RightControl,
  kKey_RightAlt,
  kKey_RightSuper,
  kKey_Menu,
};

class Window;
class InputMap;

enum InputState {
  kInactive,
  kDown,
  kPressed,
  kUp,
};

class Input {
  friend class InputMap;
public:

  ~Input() = default;

  void setState(unsigned int state);
  unsigned int getState() const;

  void setKey(int key);
  int getKey() const;
private:

  Input();
  unsigned int state_;
  int key_;
};

class InputMap {
public:
  InputMap(Window& w);
  ~InputMap();

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
  Input input_[88];

  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  inline static double scroll_x_;
  inline static double scroll_y_;

  double mouse_x_;
  double mouse_y_;
};

