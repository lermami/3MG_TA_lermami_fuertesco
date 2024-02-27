#include "Window.hpp"
#include "Engine.hpp"
#include "enum.hpp"
#include "shader_management.hpp"
#include "component_manager.hpp"
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

	//Add flags
	renderShadows_ = true;

	if (renderShadows_) {
		//Shadow	 
		//Create depth map buffer
		glGenFramebuffers(1, &depthmapFBO_);

		//Create 2D Texture as the framebuffer's depth buffer

		Texture depthMapTex(TextureTarget::kTexture_2D, TextureFormat::kDepthComponent, TextureType::kFloat);
		depthMapTex.set_min_filter(TextureFiltering::kNearest);
		depthMapTex.set_mag_filter(TextureFiltering::kNearest);
		depthmap_ = depthMapTex.LoadTexture(1024, 1024);

		//Attach the framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shadowProgram_ = CreateProgram(*this, "../assets/Shader/ShadowMap/dirlight.vs", "../assets/Shader/ShadowMap/dirlight.fs");
	}
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

	renderShadows_ = w.renderShadows_;
	depthmapFBO_ = w.depthmapFBO_;
	depthmap_ = w.depthmap_;
	shadowProgram_ = w.shadowProgram_;
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

	renderShadows_ = w.renderShadows_;

	depthmapFBO_ = w.depthmapFBO_;
	depthmap_ = w.depthmap_;
	shadowProgram_ = w.shadowProgram_;
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

void Window::renderLights() {
	auto& componentM = engine_.getComponentManager();
	auto lights = componentM.get_component_list<LightComponent>();
	auto l = lights->begin();

	unsigned int ambient_iterator = 0;
	unsigned int directional_iterator = 0;
	unsigned int point_iterator = 0;
	unsigned int spot_iterator = 0;

	for (auto& program : program_list_) {
		glUseProgram(program);
		for (; l != lights->end(); l++) {
			if (!l->has_value()) continue;
			auto& light = l->value();
			char name[64];

			//Ambient
			if (light.target_ == LightType::kAmbient) {
				sprintf_s(name, "u_ambient_light[%d].color_", ambient_iterator);
				SetVector3(program, name, light.color_);

				ambient_iterator++;
			}

			//Directional
			if (light.target_ == LightType::kDirectional) {
				sprintf_s(name, "u_directional_light[%d].color_", directional_iterator);
				SetVector3(program, name, light.color_);

				sprintf_s(name, "u_directional_light[%d].spec_color_", directional_iterator);
				SetVector3(program, name, light.spec_color_);

				sprintf_s(name, "u_directional_light[%d].direction_", directional_iterator);
				SetVector3(program, name, light.direction_);

				directional_iterator++;
			}

			//Point
			if (light.target_ == LightType::kPoint) {
				sprintf_s(name, "u_point_light[%d].pos_", point_iterator);
				SetVector3(program, name, light.pos_);

				sprintf_s(name, "u_point_light[%d].color_", point_iterator);
				SetVector3(program, name, light.color_);

				sprintf_s(name, "u_point_light[%d].spec_color_", point_iterator);
				SetVector3(program, name, light.spec_color_);

				sprintf_s(name, "u_point_light[%d].constant_", point_iterator);
				GLuint light_const = glGetUniformLocation(program, name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_point_light[%d].linear_", point_iterator);
				GLuint light_linear = glGetUniformLocation(program, name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_point_light[%d].quadratic_", point_iterator);
				GLuint light_quadractic = glGetUniformLocation(program, name);
				glUniform1f(light_quadractic, light.quadratic_);

				point_iterator++;
			}

			//Spot
			if (light.target_ == LightType::kSpot) {
				sprintf_s(name, "u_spot_light[%d].pos_", spot_iterator);
				SetVector3(program, name, light.pos_);

				sprintf_s(name, "u_spot_light[%d].color_", spot_iterator);
				SetVector3(program, name, light.color_);

				sprintf_s(name, "u_spot_light[%d].spec_color_", spot_iterator);
				SetVector3(program, name, light.spec_color_);

				sprintf_s(name, "u_spot_light[%d].direction_", spot_iterator);
				SetVector3(program, name, light.direction_);

				sprintf_s(name, "u_spot_light[%d].constant_", spot_iterator);
				GLuint light_const = glGetUniformLocation(program, name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_spot_light[%d].linear_", spot_iterator);
				GLuint light_linear = glGetUniformLocation(program, name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_spot_light[%d].quadratic_", spot_iterator);
				GLuint light_quadractic = glGetUniformLocation(program, name);
				glUniform1f(light_quadractic, light.quadratic_);

				sprintf_s(name, "u_spot_light[%d].cutoff_angle_", spot_iterator);
				GLuint light_cutoff = glGetUniformLocation(program, name);
				glUniform1f(light_cutoff, light.cutoff_angle_);

				spot_iterator++;
			}
		}
	}
}

void Window::render() {
	auto& componentM = engine_.getComponentManager();
	auto& resourceM = engine_.getResourceManager();
	auto renders = componentM.get_component_list<RenderComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>(); 

	auto r = renders->begin();
	auto t = transforms->begin();

	auto current_cam = componentM.get_component<CameraComponent>(current_cam_);
	current_cam->doRender(this);
	renderLights();

	if (renderShadows_) {
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO_);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderShadowMap(shadowProgram_);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (imguiInit_) {
			glViewport(width_*0.3f, 0, width_, height_);
		}
		else {
			glViewport(0, 0, width_, height_);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	for (; r != renders->end(); r++, t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		glUseProgram(render.program_);

		Mat4& m = transform.model_matrix_;

		m = m.Identity();
		m = m.Multiply(m.Translate(transform.pos_));
		m = m.Multiply(m.RotateX(transform.rot_.x).Multiply(m.RotateY(transform.rot_.y).Multiply(m.RotateZ(transform.rot_.z))));
		m = m.Multiply(m.Scale(transform.size_));
		m = m.Transpose();

		//Model matrix
		GLint modelMatrixLoc = glGetUniformLocation(render.program_, "u_m_matrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);
		
		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.texture_);

		GLuint uniform_loc = glGetUniformLocation(render.program_, "u_texture");
		glUniform1i(uniform_loc, 0);

		//Shadows
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthmap_);
		uniform_loc = glGetUniformLocation(render.program_, "u_depth_map");
		glUniform1i(uniform_loc, 1);
	

		glm::mat4 shadow_mat = ConfigureShaderAndMatrices();
		GLuint shadow = glGetUniformLocation(render.program_, "u_light_space_matrix");
		glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(shadow_mat));


		unsigned vertex_struct_size = (unsigned)sizeof(Vertex);
		VertexBuffer* vao = resourceM.getVertexBuffer(render.elements_buffer_);
		vao->bind();
		//Vertices
		vao->uploadFloatAttribute(0, 3, vertex_struct_size, (void*)0);
		//Normals
		vao->uploadFloatAttribute(3, 3, vertex_struct_size, (void*)(3 * sizeof(float)));
		//Uv
		vao->uploadFloatAttribute(1, 2, vertex_struct_size, (void*)(6 * sizeof(float)));
		//Color
		vao->uploadFloatAttribute(2, 4, vertex_struct_size, (void*)(8 * sizeof(float)));

		/*
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_struct_size, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, vao->get());
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_struct_size, (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, vao->get());
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_struct_size, (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, vao->get());
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertex_struct_size, (void*)(8 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, vao->get());
		glEnableVertexAttribArray(2);
		*/


		/*
		render.elements_buffer_.get()->bind(kTarget_VertexData);

		//Vertices
		render.elements_buffer_.get()->uploadFloatAttribute(0, 3, vertex_struct_size, (void*)0);
		//Normals
		render.elements_buffer_.get()->uploadFloatAttribute(3, 3, vertex_struct_size, (void*)(3 * sizeof(float)));
		//Uv
		render.elements_buffer_.get()->uploadFloatAttribute(1, 2, vertex_struct_size, (void*)(6 * sizeof(float)));
		//Color
		render.elements_buffer_.get()->uploadFloatAttribute(2, 4, vertex_struct_size, (void*)(8 * sizeof(float)));
		*/

		auto order_buffer = render.order_buffer_.get();
		order_buffer->bind(kTarget_Elements);
		glDrawElements(GL_TRIANGLES, order_buffer->size(), GL_UNSIGNED_INT, 0);
	}
}

void Window::renderShadowMap(unsigned int program) {
	auto& componentM = engine_.getComponentManager();
	auto& resourceM = engine_.getResourceManager();
	auto renders = componentM.get_component_list<RenderComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();

	auto r = renders->begin();
	auto t = transforms->begin();

	glUseProgram(program);

	for (; r != renders->end(); r++, t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		if (transform.pos_.x != 200) {
			Mat4& m = transform.model_matrix_;

			m = m.Identity();
			m = m.Multiply(m.Translate(transform.pos_));
			m = m.Multiply(m.RotateX(transform.rot_.x).Multiply(m.RotateY(transform.rot_.y).Multiply(m.RotateZ(transform.rot_.z))));
			m = m.Multiply(m.Scale(transform.size_));
			m = m.Transpose();

			//Model matrix
			GLint modelMatrixLoc = glGetUniformLocation(program, "u_m_matrix");
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

			//Shadows
			glm::mat4 shadow_mat = ConfigureShaderAndMatrices();
			GLuint shadow = glGetUniformLocation(program, "u_light_space_matrix");
			glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(shadow_mat));

			unsigned vertex_struct_size = (unsigned)sizeof(Vertex);
			VertexBuffer* vao = resourceM.getVertexBuffer(render.elements_buffer_);
			vao->bind();
			//Vertices
			vao->uploadFloatAttribute(0, 3, vertex_struct_size, (void*)0);
			//Normals
			vao->uploadFloatAttribute(3, 3, vertex_struct_size, (void*)(3 * sizeof(float)));
			//Uv
			vao->uploadFloatAttribute(1, 2, vertex_struct_size, (void*)(6 * sizeof(float)));
			//Color
			vao->uploadFloatAttribute(2, 4, vertex_struct_size, (void*)(8 * sizeof(float)));

			/*
			render.elements_buffer_.get()->bind(kTarget_VertexData);

			unsigned vertex_struct_size = (unsigned)sizeof(Vertex);

			//Vertices
			render.elements_buffer_.get()->uploadFloatAttribute(0, 3, vertex_struct_size, (void*)0);
			//Normals
			render.elements_buffer_.get()->uploadFloatAttribute(3, 3, vertex_struct_size, (void*)(3 * sizeof(float)));
			//Uv
			render.elements_buffer_.get()->uploadFloatAttribute(1, 2, vertex_struct_size, (void*)(6 * sizeof(float)));
			//Color
			render.elements_buffer_.get()->uploadFloatAttribute(2, 4, vertex_struct_size, (void*)(8 * sizeof(float)));
			*/

			auto order_buffer = render.order_buffer_.get();
			order_buffer->bind(kTarget_Elements);

			glDrawElements(GL_TRIANGLES, order_buffer->size(), GL_UNSIGNED_INT, 0);
		}
	}
}

glm::mat4 Window::ConfigureShaderAndMatrices() {
	glm::mat4 lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.01f, 500.0f);

	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 0.0f, 80.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void Window::setCurrentCam(size_t cam) {
	current_cam_ = cam;
}

size_t Window::getCurrentCam() {
	return current_cam_;
}

bool Window::getImguiStatus() {
	return imguiInit_;
}