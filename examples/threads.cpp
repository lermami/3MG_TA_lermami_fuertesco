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

  printf("a");

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
  int n_reps = 10000;

  for (int i = 0; i < n_reps; i++) {
    std::function<double()> mycall_double = [i]() { return heavyFunction(i, 1000000); };
    std::future<double> future = thread_manager.add(mycall_double);

    resultado.push_back(std::move(future));
  }

  thread_manager.waitFuture(resultado[0]);
  double num = resultado[0].get();
  std::cout << "Resultado: " << num << std::endl;
  
  /*
  std::string key = "Texture";
  std::function<double()> mycall_double = []() { return heavyFunction(0, 1000000); };
  std::future<double> future = thread_manager.addToChain(mycall_double, key);

  std::future<double> future2 = thread_manager.addToChain(mycall_double, key);

  */
  return 0;
}