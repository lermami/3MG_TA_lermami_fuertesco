#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string>
#include <vector>
#include <future>

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"
#include "thread_manager.hpp"

int retnum(int num) {
  printf("%d\n",num);
  return num;
}

int main(int, char**) {
  ThreadManager thread_manager;

  std::vector<std::future<int>> resultado;

  for (int i = 0; i < 3; i++) {
    auto mycall = [i]() { return retnum(i); };

    std::shared_ptr<std::packaged_task<int()>> task = std::make_shared<std::packaged_task<int()>>(std::move(mycall));
    std::future<int> future = task->get_future();
    thread_manager.add([task]() {(*task); });
    resultado.push_back(std::move(future));
  }
  
  system("pause");
  return 0;
}