#include <GL/glew.h>
#include "Window.hpp"
#include <time.h>

std::optional<Window> Window::create(Engine& engine, int w, int h, const char* title) {
  Window window(w, h, title);

  if (!window.is_done()) {
    return window;
  }
  else {
    return std::nullopt;
  }
}

bool Window::is_done() const {
  if (NULL == handle_) return true;
  if (glfwWindowShouldClose(handle_)) return true;

  return false;
}

void Window::swap() {
  deltaTime_ = (float)(currentTime_ - lastTime_);

  glfwPollEvents();
  glfwSwapBuffers(handle_);
}

void Window::calculateCurrentTime() {
  currentTime_ = clock();
  deltaTime_ = (static_cast<float>(currentTime_ - lastTime_) / CLOCKS_PER_SEC);
}

void Window::calculateLastTime() {
  lastTime_ = clock();
}

void Window::clearColor(float r, float g, float b, float a) const {
  glClearColor(r, g, b, a);
}

float Window::getDeltaTime() {
  return deltaTime_;
}

Window::~Window() {
  handle_ = NULL;
  
}

Window::Window(Window& w) : handle_{ w.handle_ }{
  w.handle_ = NULL;

  currentTime_ = w.currentTime_;
  lastTime_ = w.lastTime_;
  deltaTime_ = w.deltaTime_;
}

Window::Window(Window&& w) noexcept : handle_{w.handle_ }  {
  w.handle_ = NULL;

  currentTime_ = w.currentTime_;
  lastTime_ = w.lastTime_;
  deltaTime_ = w.deltaTime_;
}

Window::Window(int w, int h, const char* title) {
  handle_ = glfwCreateWindow(w, h, title, NULL, NULL);
  glfwMakeContextCurrent(handle_);
  glewInit();

  currentTime_ = 0;
  lastTime_ = 0;
  deltaTime_ = 0;

}

