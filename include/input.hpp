#pragma once

#include <vector>

#define 	KEY_W   87
#define 	KEY_A   65
#define 	KEY_S   83
#define 	KEY_D   68

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
