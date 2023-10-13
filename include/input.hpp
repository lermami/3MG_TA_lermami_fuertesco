#pragma once

#include <vector>

enum Key {
  kKey_BackSpace = 8,
  kKey_HorizontalTab,
  kKey_ShiftOut = 14,
  kKey_ShiftIn,
  kKey_Escape = 27,
  kKey_Space = 32,
  kKey_Asterisk = 42,
  kKey_Plus,
  kKey_Coma,
  kKey_Hyphen,
  kKey_Dot,
  kKey_0 = 48,
  kKey_1,
  kKey_2,
  kKey_3,
  kKey_4,
  kKey_5,
  kKey_6,
  kKey_7,
  kKey_8,
  kKey_9,
  kKey_LessThan = 60,
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
  kKey_Caret = 94,
  kKey_Delete = 127,
  kKey_Degree = 248,
  kKey_Diaeresis,  
};

class Window;

enum InputState {
  kInactive,
  kDown,
  kPressed,
  kUp,
};

class Input {
public:

  Input(int key);
  ~Input() = default;
  void update();

  bool IsKeyDown();
  bool IsKeyPressed();
  bool IsKeyUp();

  void setState(unsigned int state);
  unsigned int getState() const;

  int getKey() const;
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

class InputMap {
public:
  InputMap(Window& w);
  ~InputMap();

  void addInput(Input* new_input);
  void updateInputs();

private:
  std::vector<Input*> inputmap_;
  GLFWwindow* windowHandle_;
};
