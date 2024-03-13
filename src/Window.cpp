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
	auto maybe_window = WindowResource::create(w, h, title, imgui);

	if (!maybe_window.has_value()) {
		return std::nullopt;
	}

	WindowResource window{ std::move(*maybe_window) };

  Window a{ engine, std::move(window) ,  w, h, imgui };
	return std::move(a);
}

Window::Window(Engine& e, WindowResource&& window_resource, int w, int h, bool imgui) : engine_{ e }, window_resource_{std::move(window_resource)}{
	imguiInit_ = imgui;

	width_ = w;
	height_ = h;
	currentTime_ = 0;
	lastTime_ = 0;
	deltaTime_ = 0;

	current_cam_ = e.getComponentManager().add_entity(TransformComponent(), CameraComponent("DefaultCamera", 1.0f, 1.0f));
	e.getCameraManager().setCurrentCam(current_cam_);
}

Window::~Window() {
	if (imguiInit_) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

//Window::Window(Window& w) : handle_{ w.handle_ }, engine_{ w.engine_ }{
//	w.handle_ = NULL;
//
//	width_ = w.width_;
//	height_ = w.height_;
//	currentTime_ = w.currentTime_;
//	lastTime_ = w.lastTime_;
//	deltaTime_ = w.deltaTime_;
//
//	imguiInit_ = w.imguiInit_;
//	if (imguiInit_) {
//		initImGui();
//	}
//	w.imguiInit_ = false;
//
//	program_list_ = w.program_list_;
//	current_cam_ = w.current_cam_;
//}

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

  ImGui_ImplGlfw_InitForOpenGL(window_resource_.handle_, true);
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
  if (glfwWindowShouldClose(window_resource_.handle_)) return true;

  return false;
}

void Window::swap() {
  deltaTime_ = (float)(currentTime_ - lastTime_);

  glfwPollEvents();
  glfwSwapBuffers(window_resource_.handle_);
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
		glViewport((GLint) ((float) width_ * 0.3f), 0, width_, height_);
	}
	else {
		glViewport(0, 0, width_, height_);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


GLFResource::GLFResource() {
	glfwInit();
}

GLFResource::~GLFResource() {
	if (destroy_) {
		glfwTerminate();
	}
}

GLFResource::GLFResource(GLFResource&& other) : destroy_ {other.destroy_} {
	other.destroy_ = false;
}

GLFResource& GLFResource::operator=(GLFResource&& other) {
	destroy_ = other.destroy_;
	other.destroy_ = false;

	return *this;
}

std::optional<WindowResource> WindowResource::create(int w, int h, const char* title, bool imgui) {
	GLFResource glfw;

	auto handle = glfwCreateWindow(w + (int)((float)w * 0.3f), h, title, NULL, NULL);
	if (NULL == handle) return std::nullopt;

	glfwMakeContextCurrent(handle);

	glViewport((GLint)((float)w * 0.3f), 0, w, h);

	GLenum initstate = glewInit();

	if (initstate != GLEW_OK) {
		return std::nullopt;
	}

	return WindowResource(std::move(glfw), handle);
}

WindowResource::WindowResource(GLFResource glfw_resource, GLFWwindow* handle) : handle_{ handle }, glfw_resource_ { std::move(glfw_resource) }{
}

WindowResource::~WindowResource() {

}

WindowResource::WindowResource(WindowResource&& other) : handle_{ other.handle_ }, glfw_resource_ {std::move(other.glfw_resource_)} {
}

WindowResource& WindowResource::operator=(WindowResource&& other) {
	handle_ = other.handle_;
	glfw_resource_ = std::move(other.glfw_resource_);

	return *this;
}