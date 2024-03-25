#include "Renderer.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "component_manager.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "shader_management.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "math_library/vector_3.hpp"

Renderer::Renderer(Engine& e, Window& w) : engine_{ e }, window_{ w }
{
	//Add flags
	renderShadows_ = true;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glDepthFunc(GL_LESS);

	if (renderShadows_) {
		//Shadow	 
		//Create depth map buffer
		glGenFramebuffers(1, &depthmapFBO_);

		//Create 2D Texture as the framebuffer's depth buffer



		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		shadow_resolution_ = 2048;
		depthmap_ = Texture::LoadTexture(TextureInfo{ TextureTarget::kTexture_2D, TextureFormat::kDepthComponent, TextureType::kFloat ,
			TextureWrap::kClampToBorder,TextureWrap::kClampToBorder, TextureFiltering::kNearest ,TextureFiltering::kNearest }, shadow_resolution_, shadow_resolution_);

		//Attach the framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap_.get(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//TODO: Move this
		//auto program = Program::create(window_, "../assets/Shader/ShadowMap/dirlight.vs", "../assets/Shader/ShadowMap/dirlight.fs");

		//if (program.has_value())
		//	shadowProgram_ = std::move(program.value());
	}
}

Renderer::~Renderer()
{

}

void Renderer::renderLights() {
	auto& componentM = engine_.getComponentManager();
	auto lights = componentM.get_component_list<LightComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();


	for (int i = 0; i < window_.getProgramListSize(); i++) {
		Shader* program = window_.getProgram(i);

		unsigned int ambient_iterator = 0;
		unsigned int directional_iterator = 0;
		unsigned int point_iterator = 0;
		unsigned int spot_iterator = 0;

		glUseProgram(program->get());

		auto l = lights->begin();
		auto t = transforms->begin();

		for (; l != lights->end(); l++, t++) {
			if (!l->has_value() || !t->has_value()) continue;

			auto& light = l->value();
			auto& transform = t->value();

			char name[64];

			//Ambient
			if (light.type_ == LightType::kAmbient) {
				sprintf_s(name, "u_ambient_light[%d].pos_", point_iterator);
				program->SetVector3(name, transform.pos_);

				sprintf_s(name, "u_ambient_light[%d].color_", ambient_iterator);
				program->SetVector3(name, light.color_);

				ambient_iterator++;
			}

			//Directional
			if (light.type_ == LightType::kDirectional) {
				sprintf_s(name, "u_directional_light[%d].pos_", point_iterator);
				program->SetVector3(name, transform.pos_);

				sprintf_s(name, "u_directional_light[%d].color_", directional_iterator);
				program->SetVector3(name, light.color_);

				sprintf_s(name, "u_directional_light[%d].spec_color_", directional_iterator);
				program->SetVector3(name, light.spec_color_);

				sprintf_s(name, "u_directional_light[%d].direction_", directional_iterator);
				program->SetVector3(name, light.direction_);

				if (renderShadows_) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, depthmap_.get());
					GLuint shadow_loc = glGetUniformLocation(program->get(), "u_depth_map");
					glUniform1i(shadow_loc, 1);
				}

				directional_iterator++;
			}

			//Point
			if (light.type_ == LightType::kPoint) {
				sprintf_s(name, "u_point_light[%d].pos_", point_iterator);
				program->SetVector3(name, transform.pos_);

				sprintf_s(name, "u_point_light[%d].color_", point_iterator);
				program->SetVector3(name, light.color_);

				sprintf_s(name, "u_point_light[%d].spec_color_", point_iterator);
				program->SetVector3(name, light.spec_color_);

				sprintf_s(name, "u_point_light[%d].constant_", point_iterator);
				GLuint light_const = glGetUniformLocation(program->get(), name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_point_light[%d].linear_", point_iterator);
				GLuint light_linear = glGetUniformLocation(program->get(), name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_point_light[%d].quadratic_", point_iterator);
				GLuint light_quadractic = glGetUniformLocation(program->get(), name);
				glUniform1f(light_quadractic, light.quadratic_);

				point_iterator++;
			}

			//Spot
			if (light.type_ == LightType::kSpot) {
				sprintf_s(name, "u_spot_light[%d].pos_", spot_iterator);
				program->SetVector3(name, transform.pos_);

				sprintf_s(name, "u_spot_light[%d].color_", spot_iterator);
				program->SetVector3(name, light.color_);

				sprintf_s(name, "u_spot_light[%d].spec_color_", spot_iterator);
				program->SetVector3(name, light.spec_color_);

				sprintf_s(name, "u_spot_light[%d].direction_", spot_iterator);
				program->SetVector3(name, light.direction_);

				sprintf_s(name, "u_spot_light[%d].constant_", spot_iterator);
				GLuint light_const = glGetUniformLocation(program->get(), name);
				glUniform1f(light_const, light.constant_);

				sprintf_s(name, "u_spot_light[%d].linear_", spot_iterator);
				GLuint light_linear = glGetUniformLocation(program->get(), name);
				glUniform1f(light_linear, light.linear_);

				sprintf_s(name, "u_spot_light[%d].quadratic_", spot_iterator);
				GLuint light_quadractic = glGetUniformLocation(program->get(), name);
				glUniform1f(light_quadractic, light.quadratic_);

				sprintf_s(name, "u_spot_light[%d].cutoff_angle_", spot_iterator);
				GLuint light_cutoff = glGetUniformLocation(program->get(), name);
				glUniform1f(light_cutoff, light.cutoff_angle_);

				spot_iterator++;
			}
		}
	}
}


void Renderer::CalculateShadowsMatrix() {
	auto& componentM = engine_.getComponentManager();
	auto lights = componentM.get_component_list<LightComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();


	for (int i = 0; i < window_.getProgramListSize(); i++) {
		Shader* program = window_.getProgram(i);

		unsigned int shadow_iterator = 0;

		glUseProgram(program->get());

		auto l = lights->begin();
		auto t = transforms->begin();

		for (; l != lights->end(); l++, t++) {
			if (!l->has_value() || !t->has_value()) continue;

			auto& light = l->value();
			auto& transform = t->value();

			//Directional
			if (light.type_ == LightType::kDirectional) {

				glm::mat4 shadow_mat = ConfigureShadowMatrix(light.min_shadow_render_distance_, light.max_shadow_render_distance_, transform.pos_, light.direction_);

				GLuint shadow = glGetUniformLocation(program->get(), "u_light_space_matrix");
				glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(shadow_mat));
			}

			//Point
			if (light.type_ == LightType::kPoint) {
				
			}

			//Spot
			if (light.type_ == LightType::kSpot) {
				
			}
		}
	}

}

void Renderer::render()
{
	auto& componentM = engine_.getComponentManager();
	auto& resourceM = engine_.getResourceManager();
	auto& cameraM = engine_.getCameraManager();


	auto renders = componentM.get_component_list<RenderComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();
	auto colors = componentM.get_component_list<ColorComponent>();

	auto r = renders->begin();
	auto t = transforms->begin();
	auto c = colors->begin();

	cameraM.doRender(&window_);

	if (renderShadows_) {
		glViewport(0, 0, shadow_resolution_, shadow_resolution_);
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO_);
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		renderShadowMap(shadowProgram_.get());
		glFrontFace(GL_CW);
		glCullFace(GL_FRONT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		window_.resetViewport();
	}

	renderLights();

	for (; r != renders->end(); r++, t++, c++) {
		if (!r->has_value() || !t->has_value()) continue;
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

		//Color
		if (c->has_value()) {
			auto& color = c->value();

			GLuint uniform_color = glGetUniformLocation(render.program_, "u_color");
			glUniform4f(uniform_color, color.color_.x, color.color_.y, color.color_.z, color.color_.w);
		}

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

		IndexBuffer* indexBuffer = resourceM.getIndexBuffer(render.order_buffer_);
		indexBuffer->bind();

		glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
	}

	if (window_.getImguiStatus()) {
		renderImgui();
	}
}

void Renderer::renderShadowMap(unsigned int program)
{
	auto& componentM = engine_.getComponentManager();
	auto& resourceM = engine_.getResourceManager();
	auto renders = componentM.get_component_list<RenderComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();

	auto r = renders->begin();
	auto t = transforms->begin();

	CalculateShadowsMatrix();

	glUseProgram(program);

	for (; r != renders->end(); r++, t++) {
		if (!r->has_value() || !t->has_value()) continue;
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

			/*
			//Shadows
			glm::mat4 shadow_mat = ConfigureShadowMatrix();
			GLuint shadow = glGetUniformLocation(program, "u_light_space_matrix");
			glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(shadow_mat));*/

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

			IndexBuffer* indexBuffer = resourceM.getIndexBuffer(render.order_buffer_);
			indexBuffer->bind();

			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
		}
	}
}

glm::mat4 Renderer::ConfigureShadowMatrix(float near, float far, Vec3 pos, Vec3 direction)
{

	unsigned int window_w = 0, window_h = 0;
	window_.getWindowSize(window_w, window_h);

	float fw = ((float)window_w) / 2.0f;
	float fh = ((float)window_h) / 2.0f;



	glm::mat4 lightProjection = glm::ortho(-fw, fw, -fh, fh, near, far);

	glm::mat4 lightView = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
		glm::vec3(pos.x, pos.y, pos.z) - glm::vec3(direction.x, direction.y, direction.z),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void Renderer::renderImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
