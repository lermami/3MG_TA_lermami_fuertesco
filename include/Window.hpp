#include "GLFW/glfw3.h"
#include <optional>

class Window {
public:
  static std::optional<Window> create(int w, int h);
  bool is_done() const;


  ~Window();
  Window(Window&);
  Window(const Window&) = delete;

private:
  Window(int w, int h);
  GLFWwindow* handle_;
};