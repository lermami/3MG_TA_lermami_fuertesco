#include "Window.hpp"
#include "Engine.hpp"
#include "enum.hpp"
#include "shader_management.hpp"
#include "component_manager.hpp"
#include "camera.hpp"
#include "texture.hpp"

#include <time.h>
#include <cassert>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "AL/alc.h"

std::optional<Window> Window::create(Engine& engine, int w, int h, const char* title, bool imgui) {
  Window window(engine, w, h, title, imgui);

  if (!window.is_done()) {
    return window;
  }
  else {
    return std::nullopt;
  }
}

Window::Window(Engine& e, int w, int h, const char* title, bool imgui) : engine_{ e } {
	imguiInit_ = imgui;
	if (imguiInit_) {
		handle_ = glfwCreateWindow(w + w*0.3, h, title, NULL, NULL);
		glViewport(w * 0.3f, 0, w, h);
	}
	else {
		handle_ = glfwCreateWindow(w, h, title, NULL, NULL);
	}
	glfwMakeContextCurrent(handle_);

	GLenum initstate = glewInit();
	assert(initstate == GLEW_OK);

	width_ = w;
	height_ = h;
	currentTime_ = 0;
	lastTime_ = 0;
	deltaTime_ = 0;

	current_cam_ = e.getComponentManager().add_entity(TransformComponent(), CameraComponent("DefaultCamera", 1.0f, 1.0f));
	e.getCameraManager().setCurrentCam(current_cam_);
}

Window::~Window() {
	handle_ = NULL;

	if (imguiInit_) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

Window::Window(Window& w) : handle_{ w.handle_ }, engine_{ w.engine_ }{
	w.handle_ = NULL;

	width_ = w.width_;
	height_ = w.height_;
	currentTime_ = w.currentTime_;
	lastTime_ = w.lastTime_;
	deltaTime_ = w.deltaTime_;

	imguiInit_ = w.imguiInit_;
	if (imguiInit_) {
		initImGui();
	}
	w.imguiInit_ = false;

	program_list_ = w.program_list_;
	current_cam_ = w.current_cam_;
}

Window::Window(Window&& w) noexcept : handle_{ w.handle_ }, engine_{ w.engine_ }  {
	w.handle_ = NULL;

	width_ = w.width_;
	height_ = w.height_;
	currentTime_ = w.currentTime_;
	lastTime_ = w.lastTime_;
	deltaTime_ = w.deltaTime_;

	imguiInit_ = w.imguiInit_;
	if (imguiInit_) {
		initImGui();
	}
	w.imguiInit_ = false;

	program_list_ = w.program_list_;
	current_cam_ = w.current_cam_;
}

void Window::initSoundContext() {
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* ctx = alcCreateContext(device, NULL);
  alcMakeContextCurrent(ctx);
}

void Window::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // Setup Platform/Renderer bindings

  ImGui_ImplGlfw_InitForOpenGL(handle_, true);
  ImGui_ImplOpenGL3_Init("#version 130");
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  imguiInit_ = true;
}

void Window::updateImGui() {
	if (imguiInit_) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Window::getWindowSize(unsigned int& w, unsigned int& h) {
  w = width_;
  h = height_;
}

void Window::setwindowsize(unsigned int w, unsigned int h) {
  width_ = w;
  height_ = h;
}

void Window::enableCulling(bool enable) {
  enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void Window::enableDepthTest(bool enable) {
  enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Window::setCullingMode(CullingMode culling, FrontFace frontface) {
	GLenum glculling = GL_NONE;

	switch (culling) {
		case CullingMode::kBack: glculling = GL_BACK; break;
		case CullingMode::kBackLeft: glculling = GL_BACK_LEFT; break;
		case CullingMode::kBackRight: glculling = GL_BACK_RIGHT; break;
		case CullingMode::kFront: glculling = GL_FRONT; break;
		case CullingMode::kFrontAndBack: glculling = GL_FRONT_AND_BACK; break;
		case CullingMode::kFrontLeft: glculling = GL_FRONT_LEFT; break;
		case CullingMode::kFrontRight: glculling = GL_FRONT_RIGHT; break;
		case CullingMode::kLeft: glculling = GL_LEFT; break;
		case CullingMode::kRight: glculling = GL_RIGHT; break;
	}

	GLenum glfrontface = GL_NONE;
	switch (frontface) {
		case FrontFace::kClockWise: glfrontface = GL_CW; break;
		case FrontFace::kCounterClockWise: glfrontface = GL_CCW; break;
	}

  glCullFace(glculling);
  glFrontFace(glfrontface);
}

void Window::setDepthTestMode(DepthTestMode mode) {
	GLenum glmode = GL_NONE;

	switch (mode) {
		case DepthTestMode::kEqual: glmode = GL_EQUAL; break;
		case DepthTestMode::kGreater: glmode = GL_GREATER; break;
		case DepthTestMode::kLess: glmode = GL_LESS; break;
		case DepthTestMode::kNever: glmode = GL_NEVER; break;
	}

  glDepthFunc(glmode);
}

void Window::addProgram(unsigned new_program) {
  bool is_new = true;

  for (auto& program : program_list_) {
    if (program == new_program) {
      is_new = false;
    }
  }

  if (is_new) {
    program_list_.push_back(new_program);
  }
}

unsigned Window::getProgram(int n) {
  return program_list_.at(n);
}

int Window::getProgramListSize() {
  return (int)program_list_.size();
}

bool Window::getImguiStatus() {
	return imguiInit_;
}

void Window::resetViewport() {
	if (imguiInit_) {
		glViewport(width_ * 0.3f, 0, width_, height_);
	}
	else {
		glViewport(0, 0, width_, height_);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}