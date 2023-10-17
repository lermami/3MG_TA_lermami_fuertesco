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

void heavyFunction(int id, int iterations) {
  double result = 0.0;
  for (int i = 0; i < iterations; ++i) {
    result += sin(i) * tan(i);
  }
  std::cout << "Thread " << id << " finished with result: " << result << std::endl;
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

  std::vector<std::future<int*>> resultado;

  for (int i = 0; i < 4; i++) {
    auto mycall = []() { return CreateTexture(100000, 100000); };
    std::shared_ptr<std::packaged_task<int*()>> task = std::make_shared<std::packaged_task<int*()>>(std::move(mycall));
    std::future<int*> future = task->get_future();
    thread_manager.add([task]() {(*task)(); });
    resultado.push_back(std::move(future));
  }
  
  std::future_status status;
  do {
    switch (status = resultado[0].wait_for(1s); status) {
    case std::future_status::deferred: std::cout << " deferred\n"; break;
    case std::future_status::timeout: std::cout << " timeout\n"; break;
    case std::future_status::ready: std::cout << " ready!\n"; break;
    }
  } while (status != std::future_status::ready);

  return 0;
}