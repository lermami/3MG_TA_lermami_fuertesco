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

	//Shadow
	if (renderShadows_) {
		
		//Directional Light
		//Create depth map buffer
		glGenFramebuffers(1, &dir_depthmapFBO_);

		//Create 2D Texture as the framebuffer's depth buffer
		Texture depthMapTex(TextureTarget::kTexture_2D, TextureFormat::kDepthComponent, TextureType::kFloat);
		depthMapTex.set_min_filter(TextureFiltering::kNearest);
		depthMapTex.set_mag_filter(TextureFiltering::kNearest);
		depthMapTex.set_wrap_s(TextureWrap::kClampToBorder);
		depthMapTex.set_wrap_t(TextureWrap::kClampToBorder);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		

		shadow_resolution_ = 2048;
		dir_depthmap_ = depthMapTex.LoadTexture(shadow_resolution_, shadow_resolution_);

		//Attach the framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, dir_depthmapFBO_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dir_depthmap_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		dir_shadowProgram_ = CreateProgram(window_, "../assets/Shader/ShadowMap/dirlight.vs", "../assets/Shader/ShadowMap/dirlight.fs");
		point_shadowProgram_ = CreateProgram(window_, "../assets/Shader/ShadowMap/pointlight.vs", "../assets/Shader/ShadowMap/pointlight.fs", "../assets/Shader/ShadowMap/pointlight.gs");
		
		//Point Light
		glGenFramebuffers(1, &point_depthmapFBO_);

		glGenTextures(1, &point_depthmap_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, point_depthmap_);
		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadow_resolution_, shadow_resolution_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, point_depthmapFBO_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, point_depthmap_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 1);
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
		unsigned program = window_.getProgram(i);

		unsigned int ambient_iterator = 0;
		unsigned int directional_iterator = 0;
		unsigned int point_iterator = 0;
		unsigned int spot_iterator = 0;

		glUseProgram(program);

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
				SetVector3(program, name, transform.pos_);

				sprintf_s(name, "u_ambient_light[%d].color_", ambient_iterator);
				SetVector3(program, name, light.color_);

				ambient_iterator++;
			}

			//Directional
			if (light.type_ == LightType::kDirectional) {
				sprintf_s(name, "u_directional_light[%d].pos_", point_iterator);
				SetVector3(program, name, transform.pos_);

				sprintf_s(name, "u_directional_light[%d].color_", directional_iterator);
				SetVector3(program, name, light.color_);

				sprintf_s(name, "u_directional_light[%d].spec_color_", directional_iterator);
				SetVector3(program, name, light.spec_color_);

				sprintf_s(name, "u_directional_light[%d].direction_", directional_iterator);
				SetVector3(program, name, light.direction_);

				if (renderShadows_) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, dir_depthmap_);
					GLuint shadow_loc = glGetUniformLocation(program, "u_directional_depth_map");
					glUniform1i(shadow_loc, 1);

					sprintf_s(name, "u_directional_light[%d].far_", point_iterator);
					GLuint light_far = glGetUniformLocation(program, name);
					glUniform1f(light_far, light.max_shadow_render_distance_);
				}

				directional_iterator++;
			}

			//Point
			if (light.type_ == LightType::kPoint) {
				sprintf_s(name, "u_point_light[%d].pos_", point_iterator);
				SetVector3(program, name, transform.pos_);

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

				if (renderShadows_) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_CUBE_MAP, point_depthmap_);
					GLuint shadow_loc = glGetUniformLocation(program, "u_point_depth_map");
					glUniform1i(shadow_loc, 1);

					sprintf_s(name, "u_point_light[%d].far_", point_iterator);
					GLuint light_far = glGetUniformLocation(program, name);
					glUniform1f(light_far, light.max_shadow_render_distance_);

					
				}

				point_iterator++;
			}

			//Spot
			if (light.type_ == LightType::kSpot) {
				sprintf_s(name, "u_spot_light[%d].pos_", spot_iterator);
				SetVector3(program, name, transform.pos_);

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

				sprintf_s(name, "u_spot_light[%d].far_", point_iterator);
				GLuint light_far = glGetUniformLocation(program, name);
				glUniform1f(light_far, light.max_shadow_render_distance_);

				spot_iterator++;
			}
		}
	}
}


void Renderer::CalculateShadowsMatrix(unsigned int point_program) {
	auto& componentM = engine_.getComponentManager();
	auto lights = componentM.get_component_list<LightComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();


	for (int i = 0; i < window_.getProgramListSize(); i++) {
		unsigned program = window_.getProgram(i);

		unsigned int shadow_iterator = 0;

		glUseProgram(program);

		auto l = lights->begin();
		auto t = transforms->begin();

		for (; l != lights->end(); l++, t++) {
			if (!l->has_value() || !t->has_value()) continue;

			auto& light = l->value();
			auto& transform = t->value();

			//Directional
			if (light.type_ == LightType::kDirectional) {

				glm::mat4 dir_shadow_mat = ConfigureDirShadowMatrix(light.min_shadow_render_distance_, light.max_shadow_render_distance_, transform.pos_, light.direction_);

				GLuint shadow = glGetUniformLocation(program, "u_light_space_matrix");
				glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(dir_shadow_mat));
			}

			//Point
			if (light.type_ == LightType::kPoint) {
				DotShadowMatrix dot_shadow_mat = ConfigurePointShadowMatrix(light.min_shadow_render_distance_, light.max_shadow_render_distance_, transform.pos_, light.direction_);

				for (int i = 0; i < 6; i++) {
					char name[64];

					sprintf_s(name, "u_point_light_matrix[%d]", i);
					GLuint shadow = glGetUniformLocation(program, name);
					glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(dot_shadow_mat.m[i]));

				}

				//Shadow Matrix ----------------------> Fix Only last point light avaible
				glUseProgram(point_program);
				char name[64];

				sprintf_s(name, "u_light_pos");
				GLuint shadow_pos = glGetUniformLocation(point_program, name);
				SetVector3(point_program, name, transform.pos_);

				GLuint shadow_far = glGetUniformLocation(point_program, "u_far_plane");
				glUniform1f(shadow_far, light.max_shadow_render_distance_);

				for (int i = 0; i < 6; i++) {
					char u_name[64];

					sprintf_s(u_name, "u_point_light_matrix[%d]", i);
					GLuint shadow = glGetUniformLocation(point_program, u_name);
					glUniformMatrix4fv(shadow, 1, GL_FALSE, glm::value_ptr(dot_shadow_mat.m[i]));

				}				

				glUseProgram(program);

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
		renderShadowMap(dir_shadowProgram_, point_shadowProgram_);
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

void Renderer::renderShadowMap(unsigned int dir_program, unsigned int point_program)
{
	//Prepare Directional Light ShadowMap
	glViewport(0, 0, shadow_resolution_, shadow_resolution_);
	glBindFramebuffer(GL_FRAMEBUFFER, dir_depthmapFBO_);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//

	auto& componentM = engine_.getComponentManager();
	auto& resourceM = engine_.getResourceManager();

	auto renders = componentM.get_component_list<RenderComponent>();
	auto transforms = componentM.get_component_list<TransformComponent>();

	auto r = renders->begin();
	auto t = transforms->begin();

	CalculateShadowsMatrix(point_program);

	glUseProgram(dir_program);

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
			GLint modelMatrixLoc = glGetUniformLocation(dir_program, "u_m_matrix");
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

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

	//End directional Light shadowMap
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//

	//Prepare Point Light ShadowMap
	glViewport(0, 0, shadow_resolution_, shadow_resolution_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, point_depthmap_);

	window_.resetViewport();

	glUseProgram(point_program);

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
			GLint modelMatrixLoc = glGetUniformLocation(point_program, "u_m_matrix");
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

			//Light info


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

glm::mat4 Renderer::ConfigureDirShadowMatrix(float near, float far, Vec3 pos, Vec3 direction)
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

DotShadowMatrix Renderer::ConfigurePointShadowMatrix(float near, float far, Vec3 pos, Vec3 direction)
{

	unsigned int window_w = 0, window_h = 0;
	window_.getWindowSize(window_w, window_h);

	float fw = ((float)window_w) / 2.0f;
	float fh = ((float)window_h) / 2.0f;

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), fw / fh, near, far);

	DotShadowMatrix shadowTransforms;
	shadowTransforms.m[0] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.m[1] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.m[2] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.m[3] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.m[4] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.m[5] = glm::mat4(shadowProj * glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	return shadowTransforms;
}

void Renderer::renderImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
