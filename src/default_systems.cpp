#include "default_systems.hpp"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "sound/soundbuffer.h"
#include "shader_management.hpp"
#include "camera.hpp"
#include "input.hpp"

void init_render_component_system(RenderComponent& render, Geometry& geometry, unsigned int program, unsigned int texture) {

	for (int i = 0; i < geometry.vertex_.size(); i++) {
		render.geometry_.vertex_.push_back(geometry.vertex_[i]);
	}

	unsigned vertex_struct_size = (unsigned) sizeof(render.geometry_.vertex_[0]);
	unsigned vertex_buffer_size = render.geometry_.vertex_.size();

	render.elements_buffer_ = std::make_shared<Buffer>();
	render.elements_buffer_.get()->init(vertex_struct_size * vertex_buffer_size);
	render.elements_buffer_.get()->uploadData(&render.geometry_.vertex_[0], vertex_struct_size * vertex_buffer_size);

	render.order_buffer_ = std::make_shared<Buffer>();
	render.order_buffer_.get()->init((unsigned)geometry.indices_.size());
	render.order_buffer_.get()->uploadData(&geometry.indices_[0], (unsigned)(geometry.indices_.size() * sizeof(unsigned)));

	render.program_ = program;
	render.texture_ = texture;
}

void init_transform_system(TransformComponent& transform, Vec3& pos, Vec3& rot, Vec3& size) {
	transform.pos_ = pos;
	transform.size_ = size;
	transform.rot_ = rot;
}

void init_audio_system(AudioComponent& audio, SoundBuffer& buff, const char* label, ALfloat pos[3], ALfloat vel[3], float gain, float pitch, bool playing) {
	audio.sound_source_ = SoundSource(label, pos, vel, gain, pitch);

	audio.sound_source_.addSound(&buff);

	if(playing)
		audio.sound_source_.Play();
}

void init_color_system(RenderComponent& render, float r, float g, float b, float a) {
	for (auto& v : render.geometry_.vertex_) {
		v.color.x = r;
		v.color.y = g;
		v.color.z = b;
		v.color.w = a;
	}

}

void init_ambient_light_system(LightComponent& light, Vec3 color) {
	light.direction_ = Vec3{ 0,0,0 };
	light.pos_ = Vec3{ 0,0,0 };
	light.color_ = color;
	light.spec_color_ = Vec3{ 0,0,0 };
	light.constant_ = 0.0f;
	light.linear_ = 0.0f;
	light.quadratic_ = 0.0f;
	light.cutoff_angle_ = 0.0f;

	light.type_ = LightType::kAmbient;
}

void init_directional_light_system(LightComponent& light, Vec3 direction, Vec3 color, Vec3 specular) {
	light.direction_ = direction;
	light.pos_ = Vec3{ 0,0,0 };
	light.color_ = color;
	light.spec_color_ = specular;
	light.constant_ = 0.0f;
	light.linear_ = 0.0f;
	light.quadratic_ = 0.0f;
	light.cutoff_angle_ = 0.0f;

	light.type_ = LightType::kDirectional;
}

void init_point_light_system(LightComponent& light, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic) {
	light.direction_ = Vec3{ 0,0,0 };
	light.pos_ = position;
	light.color_ = color;
	light.spec_color_ = specular;
	light.constant_ = constant;
	light.linear_ = linear;
	light.quadratic_ = quadratic;
	light.cutoff_angle_ = 0.0f;

	light.type_ = LightType::kPoint;
}

void init_spot_light_system(LightComponent& light, Vec3 direction, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle) {
	light.direction_ = direction;
	light.pos_ = position;
	light.color_ = color;
	light.spec_color_ = specular;
	light.constant_ = constant;
	light.linear_ = linear;
	light.quadratic_ = quadratic;
	light.cutoff_angle_ = cutoff_angle;

	light.type_ = LightType::kSpot;
}

void init_camera_system(CameraComponent& cameraComp, Vec3 pos, float speed, float sensitivity) {
	cameraComp.pos_ = pos;
	cameraComp.speed_ = speed;
	cameraComp.sensitivity_ = sensitivity;
}

void move_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 mov) {

	auto r = transforms.begin();

	for (; r != transforms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.pos_ += mov;
	}

}

void rotate_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 rot) {
	auto r = transforms.begin();

	for (; r != transforms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.rot_ += rot;
	}
}

size_t on_click_system(std::vector<std::optional<TransformComponent>>& transforms, float mouse_x, float mouse_y) {

	mouse_x = (mouse_x / 1024 * 2) - 1;
	mouse_y = ((mouse_y / 768 * 2) - 1) * -1;

	auto r = transforms.begin();
	size_t e = 1;

	for (; r != transforms.end(); r++, e++) {
		if (!r->has_value()) continue;

		//TODO: Add size
		float x1 = r->value().pos_.x - r->value().size_.x;
		float x2 = r->value().pos_.x + r->value().size_.x;
		float y1 = r->value().pos_.y - r->value().size_.y;
		float y2 = r->value().pos_.y + r->value().size_.y;

		if (mouse_x > x1 && mouse_x < x2 &&
			mouse_y > y1 && mouse_y < y2)
			return e;

	}

	return 0;
}

void set_position_system(TransformComponent& transform, Vec3 pos) {
	pos.x = (pos.x / 1024 * 2) - 1;
	pos.y = ((pos.y / 768 * 2) - 1) * -1;

	transform.pos_ = pos;
}

void move_camera_system(CameraComponent& cam, Vec3 input) {

	if (input.z < 0) {
		cam.pos_ -= cam.forward_ * cam.speed_;
	}

	if (input.z > 0) {
		cam.pos_ += cam.forward_ * cam.speed_;
	}

	if (input.x > 0) {
		cam.pos_ += Vec3::CrossProduct(cam.forward_, cam.up_).Normalized();
	}

	if (input.x < 0) {
		cam.pos_ -= Vec3::CrossProduct(cam.forward_, cam.up_).Normalized();
	}

}

void rotate_camera_system(CameraComponent& cam, Input& input, const float w, const float h) {
	static float alpha = -1.57f;
	static float omega = 0;
	static float last_alpha = -1.57f;
	static float last_omega = 0;
	static Vec2 first_pos(0.0f, 0.0f);

	double mouse_x, mouse_y;
	input.getMousePos(mouse_x, mouse_y);

	if (input.IsKeyDown(kKey_LeftClick)) {
		first_pos = Vec2((float)mouse_x, (float)mouse_y);
	}

	if (input.IsKeyPressed(kKey_LeftClick)) {
		alpha = last_alpha + ((float)mouse_x - first_pos.x) / w * cam.sensitivity_;
		omega = last_omega + ((float)mouse_y - first_pos.y) / h * -1;
	}

	if (input.IsKeyUp(kKey_LeftClick)) {
		last_alpha = alpha;
		last_omega = omega;
	}

	cam.forward_.x = cos(omega) * cos(alpha);
	cam.forward_.y = sin(omega);
	cam.forward_.z = cos(omega) * sin(alpha);
}

void render_system(Window& w, CameraComponent& current_cam, std::vector<std::optional<RenderComponent>>& renders, 
	std::vector<std::optional<TransformComponent>>& transforms, std::vector<std::optional<LightComponent>>& lights) {

	auto r = renders.begin();
	auto t = transforms.begin();
	auto l = lights.begin();

	current_cam.doRender(&w);

	for (; r != renders.end(); r++, t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		//TODO: fix ecs bug
		if (render.geometry_.vertex_.size() > 0) {
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

			for (; l != lights.end(); l++) {
				auto& light = l->value();
				char name[64];

				//Ambient
				if (light.type_ == LightType::kAmbient) {
					sprintf(name, "u_ambient_light[%d].color_", ambient_iterator);
					SetVector3(render.program_, name, light.color_);

					ambient_iterator++;
				}

				//Directional
				if (light.type_ == LightType::kDirectional) {
					sprintf(name, "u_directional_light[%d].color_", directional_iterator);
					SetVector3(render.program_, name, light.color_);

					sprintf(name, "u_directional_light[%d].spec_color_", directional_iterator);
					SetVector3(render.program_, name, light.spec_color_);

					sprintf(name, "u_directional_light[%d].direction_", directional_iterator);
					SetVector3(render.program_, name, light.direction_);

					directional_iterator++;
				}

				//Point
				if (light.type_ == LightType::kPoint) {
					sprintf(name, "u_point_light[%d].pos_", point_iterator);
					SetVector3(render.program_, name, light.pos_);

					sprintf(name, "u_point_light[%d].color_", point_iterator);
					SetVector3(render.program_, name, light.color_);

					sprintf(name, "u_point_light[%d].spec_color_", point_iterator);
					SetVector3(render.program_, name, light.spec_color_);

					sprintf(name, "u_point_light[%d].constant_", point_iterator);
					GLuint light_const = glGetUniformLocation(render.program_, name);
					glUniform1f(light_const, light.constant_);

					sprintf(name, "u_point_light[%d].linear_", point_iterator);
					GLuint light_linear = glGetUniformLocation(render.program_, name);
					glUniform1f(light_linear, light.linear_);

					sprintf(name, "u_point_light[%d].quadratic_", point_iterator);
					GLuint light_quadractic = glGetUniformLocation(render.program_, name);
					glUniform1f(light_quadractic, light.quadratic_);

					point_iterator++;
				}

				//Spot
				if (light.type_ == LightType::kSpot) {
					sprintf(name, "u_spot_light[%d].pos_", spot_iterator);
					SetVector3(render.program_, name, light.pos_);

					sprintf(name, "u_spot_light[%d].color_", spot_iterator);
					SetVector3(render.program_, name, light.color_);

					sprintf(name, "u_spot_light[%d].spec_color_", spot_iterator);
					SetVector3(render.program_, name, light.spec_color_);

					sprintf(name, "u_spot_light[%d].direction_", spot_iterator);
					SetVector3(render.program_, name, light.direction_);

					sprintf(name, "u_spot_light[%d].constant_", spot_iterator);
					GLuint light_const = glGetUniformLocation(render.program_, name);
					glUniform1f(light_const, light.constant_);

					sprintf(name, "u_spot_light[%d].linear_", spot_iterator);
					GLuint light_linear = glGetUniformLocation(render.program_, name);
					glUniform1f(light_linear, light.linear_);

					sprintf(name, "u_spot_light[%d].quadratic_", spot_iterator);
					GLuint light_quadractic = glGetUniformLocation(render.program_, name);
					glUniform1f(light_quadractic, light.quadratic_);

					sprintf(name, "u_spot_light[%d].cutoff_angle_", spot_iterator);
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
}

void basic_sound_system(std::vector<std::optional<AudioComponent>>& audio_list) {
	auto l = audio_list.begin();

	ImGui::Begin("Sound information");

	int i = 0;
	for (; l != audio_list.end(); l++, i++) {
		if (!l->has_value()) continue;

		auto& sound = l->value().sound_source_;

		ImGui::PushID(i);

		bool open = ImGui::CollapsingHeader(sound.Name().c_str());

		if (open) {
			if (ImGui::Button("Play")) {
				sound.Play();
			}
			ImGui::SameLine();

			if (ImGui::Button("Stop")) {
				sound.Stop();
			}

			float aux_gain = sound.Gain();
			if (ImGui::SliderFloat("Gain", &aux_gain, 0.0f, 1.0f, "%.3f")) {
				sound.setGain(aux_gain);
			}

			float aux_p = sound.Pitch();
			if (ImGui::SliderFloat("Pitch", &aux_p, 0.0f, 2.0f, "%.3f")) {
				sound.setPitch(aux_p);
			}
		}
		ImGui::PopID();
	}

	ImGui::End();
}

void imgui_transform_system(TransformComponent& transform) {
	ImGui::Begin("Transform");

	Vec3 aux_pos = transform.pos_;
	if (ImGui::DragFloat3("Position", &aux_pos.x, 0.25f, -1000.0f, 1000.0f, "%.3f")) {
		transform.pos_ = aux_pos;
	}

	Vec3 aux_rot = transform.rot_;
	if (ImGui::DragFloat3("Rotation", &aux_rot.x, 0.1f, -100.0f, 100.0f, "%.3f")) {
		transform.rot_ = aux_rot;
	}

	float aux_size = transform.size_.x;
	if (ImGui::DragFloat("Size", &aux_size, 0.05f, 0.0f, 3.0f, "%.3f")) {
		transform.size_ = aux_size;
	}


	ImGui::End();
}
