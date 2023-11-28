#include <GLFW/glfw3.h>
#include "Input.hpp"
#include "Window.hpp"

Input::Input() {
  key_ = 0;
  state_ = InputState::kInactive;
}

void Input::setState(unsigned int state) {
  state_ = state;
}

unsigned int Input::getState() const {
  return state_;
}

void Input::setKey(int key) {
  key_ = key;
}

int Input::getKey() const {
  return key_;
}

InputMap::InputMap(Window& w) {
  windowHandle_ = w.handle_;

  scroll_x_ = 0;
  scroll_y_ = 0;
  mouse_x_ = 0;
  mouse_y_ = 0;

  //Mouse
  for (int i = 0; i < 7; i++) {
    input_[i].setKey(i);
  }

  //Individuals
  input_[7].setKey(32);
  input_[8].setKey(39);
  input_[9].setKey(44);
  input_[10].setKey(45);
  input_[11].setKey(47);
  input_[47].setKey(91);
  input_[48].setKey(93);
  input_[63].setKey(280);
  input_[64].setKey(281);
  input_[65].setKey(282);
  input_[66].setKey(284);

  //Numbers
  for (int i = 12; i < 22; i++) {
    input_[i].setKey( 48+(i-12));
  }

  //Letters
  for (int i = 22; i < 47; i++) {
    input_[i].setKey( 65 + (i - 22));
  }

  //Special keys
  for (int i = 49; i < 63; i++) {
    input_[i].setKey( 256 + (i - 49));
  }

  //F*
  for (int i = 67; i < 79; i++) {
    input_[i].setKey( 290 + (i - 67));
  }

  //Auxiliar keys
  for (int i = 79; i < 88; i++) {
    input_[i].setKey( 340 + (i - 79));
  }

}

InputMap::~InputMap() {
  
}

bool InputMap::IsKeyDown(char key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for(Input i : input_){
    if (i.getKey() == key) {
      return i.getState() == InputState::kDown;
    }
  }

  return false;
}

bool InputMap::IsKeyPressed(char key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for (Input i : input_) {
    if (i.getKey() == key) {
      return i.getState() == InputState::kPressed;
    }
  }

  return false;
}

bool InputMap::IsKeyUp(char key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for (Input i : input_) {
    if (i.getKey() == key) {
      return i.getState() == InputState::kUp;
    }
  }

  return false;
}


bool InputMap::IsKeyDown(int key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for (Input i : input_) {
    if (i.getKey() == key) {
      return i.getState() == InputState::kDown;
    }
  }

  return false;
}

bool InputMap::IsKeyPressed(int key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for (Input i : input_) {
    if (i.getKey() == key) {
      return i.getState() == InputState::kPressed;
    }
  }

  return false;
}

bool InputMap::IsKeyUp(int key) {
  int current_state = 0;

  //Get Key
  if (key >= 32) {
    current_state = glfwGetKey(windowHandle_, key);
  }
  else {
    current_state = glfwGetMouseButton(windowHandle_, key);
  }

  //Check Key
  for (Input i : input_) {
    if (i.getKey() == key) {
      return i.getState() == InputState::kUp;
    }
  }

  return false;
}

void InputMap::updateInputs() {
  for (int i = 0; i < 88; i++) {
      //Get input's state of last frame
      int last_state = input_[i].getState();
      int current_state = 0;

      //Get if input is pressed or not on this frame
      if (input_[i].getKey() >= 32) {
        current_state = glfwGetKey(windowHandle_, input_[i].getKey());
      }
      else {
        current_state = glfwGetMouseButton(windowHandle_, input_[i].getKey());
      }

      //Check key up
      if ((last_state == InputState::kPressed || last_state == InputState::kDown) && current_state == GLFW_RELEASE) {
        input_[i].setState(InputState::kUp);
      }
      else
      //Check key inactive
      if (last_state == InputState::kUp && current_state == GLFW_RELEASE) {
        input_[i].setState(InputState::kInactive);
      }
      else
      //Check key down
      if (last_state == InputState::kInactive && current_state == GLFW_PRESS) {
        input_[i].setState(InputState::kDown);
      }
      else
      //Check key pressed
      if (last_state == InputState::kDown && current_state == GLFW_PRESS) {
        input_[i].setState(InputState::kPressed);
      }
  }

  //Scroll
  if (scroll_x_ > 0) {
    scroll_x_ -= 0.5;
  }
  if (scroll_x_ < 0) {
    scroll_x_ += 0.5;
  }

  if (scroll_y_ > 0) {
    scroll_y_ -= 0.5;
  }
  if(scroll_y_ < 0){
    scroll_y_ += 0.5;
  }

  glfwSetScrollCallback(windowHandle_, scroll_callback);

  //Mouse Pos
  glfwGetCursorPos(windowHandle_, &mouse_x_, &mouse_y_);
}

void InputMap::getMousePos(double& x, double& y) {
  x = mouse_x_;
  y = mouse_y_;
}

void InputMap::setScroll(double scroll_x, double scroll_y) {
  scroll_x_ = scroll_x;
  scroll_y_ = scroll_y;
}

void InputMap::getScroll(double& scroll_x, double& scroll_y) {
  scroll_x = scroll_x_;
  scroll_y = scroll_y_;
}


void InputMap::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if ((xoffset > 0 && scroll_x_ > 0) || (xoffset < 0 && scroll_x_ < 0)) {
    scroll_x_ += xoffset;
  }
  else {
    scroll_x_ = 0 + xoffset;
  }

  if ((yoffset > 0 && scroll_y_ > 0) || (yoffset < 0 && scroll_y_ < 0)) {
    scroll_y_ += yoffset;
  }
  else {
    scroll_y_ = 0 + yoffset;
  }
}

