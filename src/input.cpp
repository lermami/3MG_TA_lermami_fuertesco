#include "Input.hpp"
#include <GLFW/glfw3.h>

Input::Input(char key) {
  key_ = int(key);
}

void Input::update(Window& w) {

  if (state_ == 1)  state_ = 2;
  if (state_ == 3)  state_ = 0;

  if (glfwGetKey(nullptr, key_) == GLFW_PRESS) {
    state_ = 1;
  }
  if (glfwGetKey(nullptr, key_) == GLFW_RELEASE) {
    state_ = 3;
  }
}

bool Input::IsKeyDown() {
  if (state_ == 1) return true;
  return false;
}

bool Input::IsKeyPressed() {
  if (state_ == 2) return true;
  return false;
}

bool Input::IsKeyUp() {
  if (state_ == 3) return true;
  return false;
}


InputMap::InputMap() {
  
}

InputMap::~InputMap() {
  inputmap_.clear();
}


void InputMap::addInput(Input new_key) {
  inputmap_.push_back(new_key);
}

void InputMap::updateInputs(Window& w) {
  for (int i = 0; i < inputmap_.size(); i++) {
    inputmap_[i].update(w);
  }
}