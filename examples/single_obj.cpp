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
#include "Renderer.hpp"
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
#include "resource_manager.hpp"

int main(int, char**) {
	Engine e;
	ThreadManager thread_manager;
	auto& component_manager = e.getComponentManager();
	auto& resourceM = e.getResourceManager();

	auto maybe_w = Window::create(e, 1024, 768, "Test Window", true);
	if (!maybe_w) return -1;
	
	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.enableCulling(true);
	w.enableDepthTest(true);
	w.setDepthTestMode(DepthTestMode::kLess);
	w.setCullingMode(CullingMode::kFront, FrontFace::kClockWise);

	Renderer renderer(e, w);

	auto simpleProgram = CreateProgram(w, "../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/laboon/laboon.obj");
	obj_paths.emplace_back("../assets/obj_test.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> mycall_vertex = [&]() { return resourceM.LoadObj("A", path.c_str()); };

		std::future<Geometry> future = thread_manager.add(mycall_vertex);

		objs.push_back(std::move(future));
	}

	Geometry laboon_geo = objs[0].get();

	unsigned n_obj = 1000;
	
	Vec3 tr_pos(0.0f, 0.0f, -6.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(1.0f, 1.0f, 1.0f);

	unsigned laboonTex = resourceM.loadTexture("Laboon", Texture(TextureTarget::kTexture_2D, TextureFormat::kRGBA, TextureType::kUnsignedByte),
																						 "../assets/laboon/laboon.png");

	unsigned wallTex = resourceM.loadTexture("Bricks", Texture(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte),
																						"../assets/wall.jpg");

	resourceM.createBuffersWithGeometry(laboon_geo, "LaboonVertices", "LaboonIndices");

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.create_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Laboon", "LaboonVertices", "LaboonIndices", simpleProgram, laboonTex);
	
  //Light
	size_t light_entity[4];

		//Ambient Light
	tr_pos = Vec3(0.0f, 0.0f, 80.0f);

	/*light_entity[0] = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity[0]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[0]);

	init_ambient_light_system(*ambient_light, Vec3(0.33f, 0.0f, 0.0f));
	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);

		//Directional Light
	light_entity[1] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[1]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[1]);

	init_directional_light_system(*ambient_light, Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);

		//Point Light
	tr_pos = Vec3(0.0f, 0.0f, -4.5f);

	light_entity[2] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[2]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[2]);

	init_point_light_system(*ambient_light, Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), 1.0f,	0.7f,	1.8f);
	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);*/

		//Spot Light
	tr_pos = Vec3(0.0f, 3.0f, -6.0f);

	light_entity[3] = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity[3]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[3]);

	init_spot_light_system(*ambient_light, Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), 1.0f,	0.0014f,	0.000007f,	0.5f);
	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	
  //Camera
	size_t main_camera = component_manager.add_entity();
	auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
	w.setCurrentCam(main_camera);

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
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
		imgui_transform_system(e, w);

		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}