#include <GLFW/glfw3.h>
#include "Input.hpp"
#include "Window.hpp"

Input::Input(InputMap& inputmap, int key) {
  key_ = key;
  state_ = 0;

  inputmap.inputmap_.push_back(this);
}

bool Input::IsKeyDown() {
  return state_ == InputState::kDown;
}

bool Input::IsKeyPressed() {
  return state_ == InputState::kPressed;
}

bool Input::IsKeyUp() {
  return state_ == InputState::kUp;
}

void Input::setState(unsigned int state) {
  state_ = state;
}

unsigned int Input::getState() const {
  return state_;
}

int Input::getKey() const {
  return key_;
}

InputMap::InputMap(Window& w) {
  windowHandle_ = w.handle_;
}

InputMap::~InputMap() {
  inputmap_.clear();
}

void InputMap::addInput(Input* new_key) {
  inputmap_.push_back(new_key);
}

void InputMap::updateInputs() {
  for (int i = 0; i < inputmap_.size(); i++) {
      //Get input's state of last frame
      int last_state = inputmap_[i]->getState();
      int current_state = 0;

      //Get if input is pressed or not on this frame
      if (inputmap_[i]->getKey() >= 32) {
        current_state = glfwGetKey(windowHandle_, inputmap_[i]->getKey());
      }
      else {
        current_state = glfwGetMouseButton(windowHandle_, inputmap_[i]->getKey());
      }

      //Check key up
      if ((last_state == InputState::kPressed || last_state == InputState::kDown) && current_state == GLFW_RELEASE) {
        inputmap_[i]->setState(InputState::kUp);
      }
      else
      //Check key inactive
      if (last_state == InputState::kUp && current_state == GLFW_RELEASE) {
        inputmap_[i]->setState(InputState::kInactive);
      }
      else
      //Check key down
      if (last_state == InputState::kInactive && current_state == GLFW_PRESS) {
        inputmap_[i]->setState(InputState::kDown);
      }
      else
      //Check key pressed
      if (last_state == InputState::kDown && current_state == GLFW_PRESS) {
        inputmap_[i]->setState(InputState::kPressed);
      }
  }

  glfwSetScrollCallback(windowHandle_, scroll_callback);
}

void InputMap::setScroll(InputMap* inputmap, double scroll_x, double scroll_y) {
  inputmap->scroll_x_ = scroll_x;
  inputmap->scroll_y_ = scroll_y;
}

void InputMap::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
 // setScroll( ,xoffset, yoffset);
}

