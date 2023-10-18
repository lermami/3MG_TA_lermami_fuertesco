#include <GLFW/glfw3.h>
#include "Input.hpp"
#include "Window.hpp"

Input::Input(int key) {
  key_ = key;
  state_ = 0;
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
    if (inputmap_[i]->getKey() > 3) {
      //Get input's state of last frame
      int last_state = inputmap_[i]->getState();
      //Get if input is pressed or not on this frame
      int current_state = glfwGetKey(windowHandle_, inputmap_[i]->getKey());

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
    else {
      //Get input's state of last frame
      int last_state = inputmap_[i]->getState();
      //Get if input is pressed or not on this frame
      int current_state = glfwGetMouseButton(windowHandle_, inputmap_[i]->getKey());//<-------------- Adaptando para mouse

      if ((last_state == GLFW_PRESS || last_state == InputState::kDown) && current_state == GLFW_RELEASE) {
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
  }
}

