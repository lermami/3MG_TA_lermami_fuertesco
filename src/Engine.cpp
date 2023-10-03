#include "Engine.hpp"
#include "GLFW/glfw3.h"


void Engine::init(){
  glfwInit();
}

void Engine::terminate(){
  glfwTerminate();
}

