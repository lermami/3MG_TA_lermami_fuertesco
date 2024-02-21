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
#include "light.hpp"
#include "camera.hpp"
#include "matrix_4.hpp"
#include "default_components.hpp"

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

	auto simpleProgram = CreateProgram(w, "../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");

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
	
	Vec3 tr_pos(0.0f, 0.0f, -6.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(1.0f, 1.0f, 1.0f);

	Texture laboon(TextureTarget::kTexture_2D, TextureFormat::kRGBA);
	unsigned laboon_handle = laboon.LoadTexture("../assets/laboon/laboon.png");

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.create_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, laboon_geo, simpleProgram, laboon_handle);
	init_vertex_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	/*
  //Light
	size_t light_entity[4];
	light_entity[0] = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity[0]);
	init_ambient_light_system(*ambient_light, Vec3(0.33f, 0.0f, 0.0f));

	light_entity[1] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[1]);
	init_directional_light_system(*ambient_light, Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	light_entity[2] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[2]);
	init_point_light_system(*ambient_light, Vec3(0.0f, 0.0f, -4.5f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), 1.0f,	0.7f,	1.8f);

	light_entity[3] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[3]);
	init_spot_light_system(*ambient_light, Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 3.0f, -6.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), 1.0f,	0.0014f,	0.000007f, 0.9f);
	*/

  //Camera
	size_t main_camera = component_manager.add_entity();
	auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
	w.setCurrentCam(main_camera);

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done()) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		Vec3 input;
		float rotate = 1;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input.z = input_velocity;
		}

		if (input_map.IsKeyPressed('S')) {
			input.z = -input_velocity;
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
		
		move_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input);
		rotate_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input_map, 1024, 768);
		imgui_transform_system(*component_manager.get_component<TransformComponent>(new_e));

		w.render(0);

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}