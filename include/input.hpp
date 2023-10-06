#pragma once

#include <vector>

#define 	KEY_W   87
#define 	KEY_A   65
#define 	KEY_S   83
#define 	KEY_D   68

class Window;

class Input {
public:

  Input(char key);
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

class InputMap {
public:
  InputMap();
  ~InputMap();

  void addInput(Input new_input);
  void updateInputs(Window& w);

private:
  std::vector<Input> inputmap_;
};