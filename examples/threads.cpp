#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>
#include <vector>
#include <future>
#include <iostream>
#include <chrono>
#include <typeinfo>
using namespace std::chrono_literals;

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"
#include "thread_manager.hpp"

double heavyFunction(int id, int iterations) {
  double result = 0.0;
  for (int i = 0; i < iterations; ++i) {
    result += sin(i) * tan(i);
  }
  return result;
}

int* CreateTexture(int w, int h) {
  int* t = new int[w*h]();

  for (int i = 0; i < w * h; ++i) {
    t[i] = i;
  }
  return t;
}

int main(int, char**) {
  ThreadManager thread_manager;

  std::vector<std::future<double>> resultado;

  for (int i = 0; i < 32; i++) {
    std::function<double()> mycall_double = [i]() { return heavyFunction(i, 100000000); };
    std::future<double> future = thread_manager.add(mycall_double);

    resultado.push_back(std::move(future));
  }

  thread_manager.waitFuture(resultado[0]);
  double num = resultado[0].get();
  std::cout << "Resultado: " << num << std::endl;
  
  return 0;
}