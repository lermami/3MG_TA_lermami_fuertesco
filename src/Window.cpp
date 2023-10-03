#include "Window.hpp"

std::optional<Window> Window::create(int w, int h) {
  Window window(w, h);

  if (!window.is_done()) {
    return window;
  }
  else {
    return std::nullopt;
  }
}

bool Window::is_done() const {
  if (NULL == handle_) return true;

  return false;
}

void Window::render() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(handle_);
  glfwPollEvents();
}


Window::~Window() {

}

Window::Window(Window& w) : handle_{ w.handle_ }{
  w.handle_ = NULL;
}

Window::Window(Window&& w) noexcept : handle_{w.handle_ } {
  w.handle_ = NULL;
}

Window::Window(int w, int h) {
  handle_ = glfwCreateWindow(w, h, "My Title", NULL, NULL);
  glfwMakeContextCurrent(handle_);
}