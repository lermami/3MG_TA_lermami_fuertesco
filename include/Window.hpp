#pragma once
#include "GLFW/glfw3.h"
#include <optional>
#include <vector>
#include <ctime>

enum class FrontFace {
  kClockWise = 0x0900,
  kCounterClockWise = 0x0901
};

enum class CullingMode {
  kFrontLeft = 0x0400,
  kFrontRight = 0x0401,
  kBackLeft = 0x0402,
  kBackRight = 0x0403,
  kFront = 0x0404,
  kBack = 0x0405,
  kLeft = 0x0406,
  kRight = 0x0407,
  kFrontAndBack = 0x0408,
};

class Engine;

class Window {
public:
  friend class Input;

  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window");
  bool is_done() const;
  void swap();
  void clearColor(float r, float g, float b, float a) const;

  void initImGui();
  void updateImGui();

  void initSoundContext();

  float getDeltaTime();
  void calculateCurrentTime();
  void calculateLastTime();

  void enableCulling(bool enable);
  void setCulling(CullingMode culling, FrontFace frontface);
  void enableDepthTest(bool enable);

  void getWindowSize(unsigned int& w, unsigned int& h);

  void setwindowsize(unsigned int w, unsigned int h);

  ~Window();
  Window(Window& w);
  Window(Window&& w) noexcept;
  Window(const Window&) = delete;

private:
  Window(int w, int h, const char* title);
  void renderImgui();

  GLFWwindow* handle_;
  clock_t  currentTime_;
  clock_t  lastTime_;
  unsigned int width_;
  unsigned int height_;
  float deltaTime_;
  bool imguiInit_;
};

