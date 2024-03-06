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
#include "Engine.hpp"
#include "enum.hpp"
#include <vector>

void init_render_component_system(RenderComponent& render, const char* name, std::string vertexBuffer, std::string orderBuffer, unsigned int program, unsigned int texture) {
	render.name_ = name;
	render.elements_buffer_ = vertexBuffer;
	render.order_buffer_ = orderBuffer;
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

void init_color_system(ColorComponent& color, float r, float g, float b, float a) {
	color.color_.x = r;
	color.color_.y = g;
	color.color_.z = b;
	color.color_.w = a;
}

void change_color_system(Engine& e, size_t entity, float r, float g, float b, float a) {
	auto& component_manager = e.getComponentManager();

	auto color = component_manager.get_component<ColorComponent>(entity);

	//----------------------------------->TODO: Check if color exist
	color->color_.x = r;
	color->color_.y = g;
	color->color_.z = b;
	color->color_.w = a;
}

void init_ambient_light_system(LightComponent& light, Vec3 color) {
	light.direction_ = Vec3{ 0,0,0 };
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
	light.color_ = color;
	light.spec_color_ = specular;
	light.constant_ = 0.0f;
	light.linear_ = 0.0f;
	light.quadratic_ = 0.0f;
	light.cutoff_angle_ = 0.0f;
	light.min_shadow_render_distance_ = 0.01f;
	light.max_shadow_render_distance_ = 1000.0f;

	light.type_ = LightType::kDirectional;
}

void init_point_light_system(LightComponent& light, Vec3 color, Vec3 specular, float constant, float linear, float quadratic) {
	light.direction_ = Vec3{ 0,0,0 };
	light.color_ = color;
	light.spec_color_ = specular;
	light.constant_ = constant;
	light.linear_ = linear;
	light.quadratic_ = quadratic;
	light.cutoff_angle_ = 0.0f;

	light.type_ = LightType::kPoint;
}

void init_spot_light_system(LightComponent& light, Vec3 direction, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle) {
	light.direction_ = direction;
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

void set_transform_system(Engine& e, size_t entity, Vec3 position, Vec3 rotation, Vec3 size) {
	auto& component_manager = e.getComponentManager();

	auto transform = component_manager.get_component<TransformComponent>(entity);

	//------------------------------->TO DO:Check that trabnsform exist
	transform->pos_ = position;
	transform->rot_ = rotation;
	transform->size_ = size;
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

	if (input.IsKeyDown(kKey_RightClick)) {
		first_pos = Vec2((float)mouse_x, (float)mouse_y);
	}

	if (input.IsKeyPressed(kKey_RightClick)) {
		alpha = last_alpha + ((float)mouse_x - first_pos.x) / w * cam.sensitivity_;
		omega = last_omega + ((float)mouse_y - first_pos.y) / h * -1;
	}

	if (input.IsKeyUp(kKey_RightClick)) {
		last_alpha = alpha;
		last_omega = omega;
	}

	cam.forward_.x = cos(omega) * cos(alpha);
	cam.forward_.y = sin(omega);
	cam.forward_.z = cos(omega) * sin(alpha);
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

void imgui_transform_system(Engine& e, Window& w) {
	if (w.getImguiStatus()) {
		auto& compM = e.getComponentManager();

		auto transformList = compM.get_component_list<TransformComponent>();
		auto renderList = compM.get_component_list<RenderComponent>();
		auto lightList = compM.get_component_list<LightComponent>();

		auto tr = transformList->begin();
		auto r = renderList->begin();
		auto l = lightList->begin();

		auto& resourceM = e.getResourceManager();
		//std::vector<unsigned>& texList = resourceM.getTextureList();
		//std::vector<std::string>& texNameList = resourceM.getTextureNamesList();
		//std::string vectorStr(texList.begin(), texList.end());

		int num = 0;

		//Init table
		unsigned w_width, w_height;
		w.getWindowSize(w_width, w_height);
		ImGui::SetNextWindowSize(ImVec2(w_width * 0.3f, w_height));
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::Begin("ImGui", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		ImGui::BeginTabBar("Tabs");
		if (ImGui::BeginTabItem("Components")) {
			for (; tr != transformList->end(); tr++, r++) {
				if (!tr->has_value() || !r->has_value()) continue;
				auto& transform = tr->value();
				auto& render = r->value();

				//Component
				ImGui::PushID(num);
				if (ImGui::CollapsingHeader(render.name_.c_str())) {

					//Transform
					if (ImGui::CollapsingHeader("Transform")) {
						Vec3 aux_pos = transform.pos_;
						if (ImGui::DragFloat3("Position", &aux_pos.x, 0.25f, -1000.0f, 1000.0f, "%.3f")) {
							transform.pos_ = aux_pos;
						}

						Vec3 aux_rot = transform.rot_;
						if (ImGui::DragFloat3("Rotation", &aux_rot.x, 0.1f, -100.0f, 100.0f, "%.3f")) {
							transform.rot_ = aux_rot;
						}

						float aux_size = transform.size_.x;
						if (ImGui::DragFloat("Size", &aux_size, 0.05f, 0.0f, 1000.0f, "%.3f")) {
							transform.size_ = aux_size;
						}
					}
					/*
					//Render
					if (ImGui::CollapsingHeader("Render")) {
						unsigned tex_aux = render.texture_;

						if (ImGui::BeginCombo("Lista desplegable", resourceM.getTextureName(tex_aux).c_str()))
						{
							for (int i = 0; i < texList.size(); i++)
							{
								if (ImGui::Selectable(texNameList[i].c_str(), texList[i] == tex_aux))
								{
									render.texture_ = texList[i];
								}
							}
							ImGui::EndCombo();
						}
					}
					*/

				}

				ImGui::PopID();
				num++;
			}

			num = 0;

			tr = transformList->begin();
			for (; l != lightList->end(); l++, tr++) {
				if (!l->has_value() || !tr->has_value()) continue;

				ImGui::PushID(num);
				auto& light = l->value();
				auto& transform = tr->value();

				if (ImGui::CollapsingHeader("Light")) {
					Vec3 aux_pos = transform.pos_;
					if (ImGui::DragFloat3("Position", &aux_pos.x, 0.25f, -1000.0f, 1000.0f, "%.3f")) {
						transform.pos_ = aux_pos;
					}

					if (light.type_ == LightType::kDirectional || light.type_ == LightType::kSpot) {
						Vec3 aux_dir = light.direction_;
						if (ImGui::DragFloat3("Direction", &aux_dir.x, 0.01f, -1.0f, 1.0f, "%.3f")) {
							light.direction_ = aux_dir;
						}
					}

					Vec3 aux_color = light.color_;
					if (ImGui::ColorPicker3("Color", &aux_color.x)) {
						light.color_ = aux_color;
					}
				}
				ImGui::PopID();
				num++;
			}
				ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Tools")) {
			/*
			static unsigned tex = resourceM.getTexture(0);
			static std::string vbuff = "Vertex";
			static std::string ibuff;
			if (ImGui::BeginCombo("Lista desplegable", resourceM.getTextureName(tex).c_str()))
			{
				for (int i = 0; i < texList.size(); i++)
				{
					if (ImGui::Selectable(texNameList[i].c_str(), texList[i] == tex))
					{
						tex = texList[i];
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Add entity")) {
				Vec3 tr_pos(0.0f, 0.0f, -6.0f);
				Vec3 obj_rot(0.0f, 1.57f, 0.0f);
				Vec3 obj_size(1.0f, 1.0f, 1.0f);

				size_t new_e = compM.add_entity();
				auto tr_render = compM.create_component<RenderComponent>(new_e);
				auto tr_transform = compM.create_component<TransformComponent>(new_e);

				init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
				init_render_component_system(*tr_render, "Laboon", "LaboonVertices", "LaboonIndices", 6, 2);
			}
			*/

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		ImGui::End();
	}

}

void init_box_collider_system(BoxColliderComponent& component, Vec3 extent, Vec3 center_offset) {
	component.extent_ = extent;
	component.center_offset_ = center_offset;
}

bool are_colliding_system(Engine& e, size_t entity1, size_t entity2) {
	auto& component_manager = e.getComponentManager();

	//Colliders
	auto collider1 = component_manager.get_component<BoxColliderComponent>(entity1);
	auto collider2 = component_manager.get_component<BoxColliderComponent>(entity2);

	//Transform
	auto transform1 = component_manager.get_component<TransformComponent>(entity1);
	auto transform2 = component_manager.get_component<TransformComponent>(entity2);
	
	/*if (!collider1->has_value() || !collider2->has_value()//<----------------------------Fix Me PLS
		|| !transform1->has_value() || !transform2->has_value()){
		return false;
		};*/
	
	Vec3 obj1 = transform1->pos_ + collider1->center_offset_;
	Vec3 obj2 = transform2->pos_ + collider2->center_offset_;

	
		// check the X axis
		if (glm::abs(obj1.x - obj2.x) < collider1->extent_.x + collider2->extent_.x) {
			// check the Y axis
			if (glm::abs(obj1.y - obj2.y) < collider1->extent_.y + collider2->extent_.y) {
				// check the Z axis
				if (glm::abs(obj1.z - obj2.z) < collider1->extent_.z + collider2->extent_.z) {
					return true;
				}
			}
		}
		
		return false;
}
