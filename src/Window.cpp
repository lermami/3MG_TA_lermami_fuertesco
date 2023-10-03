#include "Window.hpp"


std::optional<Window> Window::create(int w, int h) {
  Window window(w, h);

  if (window.is_done()) {
    return window;
  }
  else {
    return std::nullopt;
  }
}

bool Window::is_done() const {
  if (NULL == handle_) return false;

  return true;
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
}