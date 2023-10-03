#include "Window.hpp"

std::optional<Window> Window::create(int w, int h, const char* title) {
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

void Window::render() const{
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(handle_);
  glfwPollEvents();
}

void Window::init(float r, float g, float b, float a) const {
  glfwMakeContextCurrent(handle_);
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
}