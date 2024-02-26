#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>
#include<fstream>
#include<iostream>

#include "Window.hpp"
#include "component_manager.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "camera.hpp"

#include "matrix_4.hpp"

int main(int, char**) {
	Engine e;
	ThreadManager thread_manager;
	auto& component_manager = e.getComponentManager();

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.initImGui();
	w.enableCulling(true);
	w.enableDepthTest(true);
	w.setDepthTestMode(DepthTestMode::kLess);
	w.setCullingMode(CullingMode::kFront, FrontFace::kClockWise);

	auto simpleProgram = CreateProgram(w, "../assets/test_shader/test.vs", "../assets/test_shader/test.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/Suzanne.obj");
	obj_paths.emplace_back("../assets/wolf/Wolf_obj.obj");
	obj_paths.emplace_back("../assets/gun/Gun.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> loadobj_func = [path]() { return Engine::LoadObj(path.c_str()); };

		std::future<Geometry> future = thread_manager.add(loadobj_func);

		objs.push_back(std::move(future));
	}

	Geometry suzanneGeo = objs[0].get();
	Geometry wolfGeo = objs[1].get();
	Geometry tankGeo = objs[2].get();

	unsigned n_obj = 1000;

	for (unsigned i = 0; i < n_obj / 3; i++) {
		Vec3 tr_pos;
		tr_pos.x = (rand() % 150) - 75;
		tr_pos.y = (rand() % 150) - 75;
		tr_pos.z = -100.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(2.0f, 2.0f, 2.0f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.create_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_render_component_system(*tr_render, suzanneGeo, simpleProgram, 0);
		init_vertex_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	for (unsigned i = n_obj / 3; i < 2 * n_obj / 3; i++) {
		Vec3 tr_pos;
		tr_pos.x = (rand() % 28) - 14;
		tr_pos.y = (rand() % 28) - 14;
		tr_pos.z = -20.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(1.0f, 1.0f, 1.0f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.create_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_render_component_system(*tr_render, wolfGeo, simpleProgram, 0);
		init_vertex_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	for (unsigned i = 2 * n_obj / 3; i < n_obj; i++) {
		Vec3 tr_pos;
		tr_pos.x = (rand() % 100) - 50;
		tr_pos.y = (rand() % 100) - 50;
		tr_pos.z = -60.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(5.0f, 5.0f, 5.0f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.create_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_render_component_system(*tr_render, tankGeo, simpleProgram, 0);
		init_vertex_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	//Camera
	size_t main_camera = component_manager.add_entity();
	auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
	w.setCurrentCam(main_camera);

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		Vec3 input;
		float rotate = 0;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input.z = input_velocity * 10.0f;
		}

		if (input_map.IsKeyPressed('S')) {
			input.z = -input_velocity * 10.0f;
		}

		if (input_map.IsKeyPressed('A')) {
			input.x = -input_velocity;
		}

		if (input_map.IsKeyPressed('D')) {
			input.x = input_velocity;
		}

		if (input_map.IsKeyPressed('Q')) {
			input.y = -input_velocity;
		}

		if (input_map.IsKeyPressed('E')) {
			input.y = input_velocity;
		}

		if (input_map.IsKeyPressed(kKey_Right)) {
			rotate = -input_velocity * 2.0f;
		}

		if (input_map.IsKeyPressed(kKey_Left)) {
			rotate = input_velocity * 2.0f;
		}

		if (input_map.IsKeyDown(kKey_LeftClick)) {
			//clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (input_map.IsKeyUp(kKey_LeftClick)) {
			clicked_e = 0;
		}

		//if (clicked_e != 0)
			//set_position_system(*component_manager.get_component<TransformComponent>(clicked_e), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		//move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, input_z));

		move_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input);
		rotate_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input_map, 1024, 768);
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));

		w.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}