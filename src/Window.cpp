#include "Window.hpp"

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

void Window::swap() const{
  glfwPollEvents();
  glfwSwapBuffers(handle_);
}

void Window::init(float r, float g, float b, float a) const {
  glClearColor(r, g, b, a);
}

Window::~Window() {
  handle_ = NULL;
}

Window::Window(Window& w) : handle_{ w.handle_ }{
  w.handle_ = NULL;
}

Window::Window(Window&& w) : handle_{w.handle_ } {
  w.handle_ = NULL;
}

Window::Window(int w, int h, const char* title) {
  handle_ = glfwCreateWindow(w, h, title, NULL, NULL);
  glfwMakeContextCurrent(handle_);
}

Window::Input::Input(int key) {
  key_ = key;
}
