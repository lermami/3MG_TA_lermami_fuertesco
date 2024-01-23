#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include <unordered_map>

#include "component_manager.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "texture.hpp"
using namespace std::chrono_literals;

#include "matrix_4.hpp"

int main(int, char**) {
	Engine e;
	ThreadManager thread_manager;
	ComponentManager component_manager;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.initImGui();
	w.enableCulling(true);
	w.enableDepthTest(true);
	w.setCulling(CullingMode::kBack, FrontFace::kClockWise);

	auto simpleProgram = CreateProgram("../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/laboon/laboon.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> mycall_vertex = [path]() { return Engine::LoadObj(path.c_str()); };

		std::future<Geometry> future = thread_manager.add(mycall_vertex);

		objs.push_back(std::move(future));
	}

	Geometry laboon_geo = objs[0].get();

	unsigned n_obj = 1000;
	
	Vec3 tr_pos(0.0f, 0.0f, -2.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(0.007f, 0.007f, 0.007f);

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.get_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.get_component<TransformComponent>(new_e);

	Texture laboon(TextureType::kTexture_2D, TextureFormat::kRGBA);
	unsigned laboon_handle = laboon.LoadTexture("../assets/laboon/laboon.png");

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, laboon_geo, simpleProgram, laboon_handle);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done()) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		float input_x = 0, input_y = 0;
		float rotate = 1;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input_y = input_velocity;
		}

		if (input_map.IsKeyPressed('S')) {
			input_y = -input_velocity;
		}

		if (input_map.IsKeyPressed('A')) {
			input_x = -input_velocity;
		}

		if (input_map.IsKeyPressed('D')) {
			input_x = input_velocity;
		}
		
		imgui_transform_system(*component_manager.get_component<TransformComponent>(new_e));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		//rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(rotate * w.getDeltaTime(), rotate * w.getDeltaTime(), 0.0f));
		shader_prop_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}