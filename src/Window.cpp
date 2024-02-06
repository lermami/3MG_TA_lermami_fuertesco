#include "Window.hpp"
#include "Engine.hpp"
#include "enum.hpp"
#include "shader_management.hpp"
#include "component_manager.hpp"

#include <time.h>
#include <cassert>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "AL/alc.h"

std::optional<Window> Window::create(Engine& engine, int w, int h, const char* title) {
  Window window(engine, w, h, title);

  if (!window.is_done()) {
    return window;
  }
  else {
    return std::nullopt;
  }
}

Window::Window(Engine& e, int w, int h, const char* title) : engine_{ e } {
	handle_ = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwMakeContextCurrent(handle_);

	GLenum initstate = glewInit();
	assert(initstate == GLEW_OK);

	width_ = w;
	height_ = h;
	currentTime_ = 0;
	lastTime_ = 0;
	deltaTime_ = 0;

	imguiInit_ = false;
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
}

Window::Window(Window&& w) noexcept : handle_{ w.handle_ }, engine_{ w.engine_ }  {
	w.handle_ = NULL;

	width_ = w.width_;
	height_ = w.height_;
	currentTime_ = w.currentTime_;
	lastTime_ = w.lastTime_;
	deltaTime_ = w.deltaTime_;

	imguiInit_ = w.imguiInit_;
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
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Window::renderImgui() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Window::is_done() const {
  if (NULL == handle_) return true;
  if (glfwWindowShouldClose(handle_)) return true;

  return false;
}

void Window::swap() {
  deltaTime_ = (float)(currentTime_ - lastTime_);

  if (imguiInit_) renderImgui();

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
  glCullFace((GLenum)culling);
  glFrontFace((GLenum)frontface);
}

void Window::setDepthTestMode(DepthTestMode mode) {
  glDepthFunc((GLenum)mode);
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

void Window::render() {
	auto& componentM = engine_.getComponentManager();
	auto renders = componentM.get_component_list<RenderComponent>(); //renders.begin();
	auto transforms = componentM.get_component_list<TransformComponent>(); //transforms.begin();
	auto lights = componentM.get_component_list<LightComponent>(); //lights.begin();

	auto r = renders->begin();
	auto t = transforms->begin();
	auto l = lights->begin();

	auto current_cam = componentM.get_component<CameraComponent>(current_cam_);
	current_cam->doRender(this);

	for (; r != renders->end(); r++, t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		Mat4& m = transform.model_matrix_;

		m = m.Identity();
		m = m.Multiply(m.Translate(transform.pos_));
		m = m.Multiply(m.RotateX(transform.rot_.x).Multiply(m.RotateY(transform.rot_.y).Multiply(m.RotateZ(transform.rot_.z))));
		m = m.Multiply(m.Scale(transform.size_));
		m = m.Transpose();

		glUseProgram(render.program_);
		//Model matrix
		GLint modelMatrixLoc = glGetUniformLocation(render.program_, "u_m_matrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

		//Texture
		glUniform1ui(glGetUniformLocation(render.texture_, "u_texture"), 0);

		//Light
		unsigned int ambient_iterator = 0;
		unsigned int directional_iterator = 0;
		unsigned int point_iterator = 0;
		unsigned int spot_iterator = 0;

		for (; l != lights->end(); l++) {
			if (!l->has_value()) continue;
			auto& light = l->value();
			char name[64];

			//Ambient
			if (light.type_ == LightType::kAmbient) {
				sprintf_s(name, "u_ambient_light[%d].color_", ambient_iterator);
				SetVector3(render.program_, name, light.color_);

				ambient_iterator++;
			}

			//Directional
			if (light.type_ == LightType::kDirectional) {
				sprintf_s(name, "u_directional_light[%d].color_", directional_iterator);
				SetVector3(render.program_, name, light.color_);

				sprintf_s(name, "u_directional_light[%d].spec_color_", directional_iterator);
				SetVector3(render.program_, name, light.spec_color_);

				sprintf_s(name, "u_directional_light[%d].direction_", directional_iterator);
				SetVector3(render.program_, name, light.direction_);

				directional_iterator++;
			}

			//Point
			if (light.type_ == LightType::kPoint) {
				sprintf_s(name, "u_point_light[%d].pos_", point_iterator);
				SetVector3(render.program_, name, light.pos_);

				sprintf_s(name, "u_point_light[%d].color_", point_iterator);
				SetVector3(render.program_, name, light.color_);

				sprintf_s(name, "u_point_light[%d].spec_color_", point_iterator);
				SetVector3(render.program_, name, light.spec_color_);

				sprintf_s(name, "u_point_light[%d].constant_", point_iterator);
				GLuint light_const = glGetUniformLocation(render.program_, name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_point_light[%d].linear_", point_iterator);
				GLuint light_linear = glGetUniformLocation(render.program_, name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_point_light[%d].quadratic_", point_iterator);
				GLuint light_quadractic = glGetUniformLocation(render.program_, name);
				glUniform1f(light_quadractic, light.quadratic_);

				point_iterator++;
			}

			//Spot
			if (light.type_ == LightType::kSpot) {
				sprintf_s(name, "u_spot_light[%d].pos_", spot_iterator);
				SetVector3(render.program_, name, light.pos_);

				sprintf_s(name, "u_spot_light[%d].color_", spot_iterator);
				SetVector3(render.program_, name, light.color_);

				sprintf_s(name, "u_spot_light[%d].spec_color_", spot_iterator);
				SetVector3(render.program_, name, light.spec_color_);

				sprintf_s(name, "u_spot_light[%d].direction_", spot_iterator);
				SetVector3(render.program_, name, light.direction_);

				sprintf_s(name, "u_spot_light[%d].constant_", spot_iterator);
				GLuint light_const = glGetUniformLocation(render.program_, name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_spot_light[%d].linear_", spot_iterator);
				GLuint light_linear = glGetUniformLocation(render.program_, name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_spot_light[%d].quadratic_", spot_iterator);
				GLuint light_quadractic = glGetUniformLocation(render.program_, name);
				glUniform1f(light_quadractic, light.quadratic_);

				sprintf_s(name, "u_spot_light[%d].cutoff_angle_", spot_iterator);
				GLuint light_cutoff = glGetUniformLocation(render.program_, name);
				glUniform1f(light_cutoff, light.cutoff_angle_);

				spot_iterator++;
			}
		}

		render.elements_buffer_.get()->bind(kTarget_VertexData);
		unsigned vertex_struct_size = (unsigned)sizeof(render.geometry_.vertex_[0]);

		//Vertices
		render.elements_buffer_.get()->uploadFloatAttribute(0, 3, vertex_struct_size, (void*)0);
		//Normals
		render.elements_buffer_.get()->uploadFloatAttribute(3, 3, vertex_struct_size, (void*)(3 * sizeof(float)));
		//Uv
		render.elements_buffer_.get()->uploadFloatAttribute(1, 2, vertex_struct_size, (void*)(6 * sizeof(float)));
		//Color
		render.elements_buffer_.get()->uploadFloatAttribute(2, 4, vertex_struct_size, (void*)(8 * sizeof(float)));

		auto order_buffer = render.order_buffer_.get();
		order_buffer->bind(kTarget_Elements);
		glDrawElements(GL_TRIANGLES, order_buffer->size(), GL_UNSIGNED_INT, 0);
	}
}

void Window::setCurrentCam(size_t cam) {
	current_cam_ = cam;
}

size_t Window::getCurrentCam() {
	return current_cam_;
}