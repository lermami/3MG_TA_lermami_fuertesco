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

//TO DO: fix system
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

void imgui_interface_system(Engine& e, Window& w) {
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
		ImGui::SetNextWindowSize(ImVec2((float) w_width * 0.3f, (float) w_height));
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