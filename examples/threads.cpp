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

std::string print_string(std::string s) {
  printf("%s\n", s.c_str());

  return s;
}

int main(int, char**) {
  ThreadManager thread_manager;

  std::vector<std::string> comida{ "patata.jpg", "melon.jpg", "zanahoria.jpg", "pimiento.jpg" };
  std::vector<std::future<std::string>> resultado;

  for (auto& ente : comida) {
    auto mycall = [ente]() { return print_string(ente); };
    std::shared_ptr<std::packaged_task<std::string()>> task = std::make_shared<std::packaged_task<std::string()>>(std::move(mycall));
    std::future<std::string> future = task->get_future();
    thread_manager.add([task]() {(*task)(); });
    resultado.push_back(std::move(future));
  }

  return 0;
}