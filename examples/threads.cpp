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

class Texture {};
Texture load_texture(std::string texture);

int main(int, char**) {
  Engine e;
  ThreadManager thread_manager;

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.init(0.4, 0.4, 0.4, 1);

  if (glewInit() != GLEW_OK) return -1;

  std::vector<std::string> comida{ "patata.jpg", "melon.jpg", "zanahoria.jpg", "pimiento.jpg" };
  std::vector<std::future<Texture>> resultado;
  for (auto& ente : comida) {
    auto mycall = [ente]() { return load_texture(ente); };
    auto task = std::packaged_task<Texture()>(std::move(mycall));
    std::future<Texture> future = task.get_future();
    thread_manager.add(std::move(task));
    resultado.push_back(std::move(future));
  }

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);


    w.swap();
    w.calculateCurrentTime();
  }

  return 0;
}