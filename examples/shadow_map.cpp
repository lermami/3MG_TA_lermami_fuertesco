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
#include "Renderer.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "camera.hpp"

using namespace std::chrono_literals;

#include "matrix_4.hpp"

void check_GL(const std::string& text, const char* file, int line) {
	if (auto err = glGetError(); err != GL_NO_ERROR) {
		printf("%s: %s(%d) %s\n", text.c_str(), file, line, gluErrorString(err));
	}
}

#define CHECKGL(s) check_GL(s,__FILE__,__LINE__)

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
	auto simpleProgram3 = CreateProgram(w, "../assets/Shader/ShadowMap/depthtest.vs", "../assets/Shader/ShadowMap/depthtest.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/obj_test.obj");
	obj_paths.emplace_back("../assets/square.obj");
	obj_paths.emplace_back("../assets/laboon/laboon.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> mycall_vertex = [&]() { return resourceM.LoadObj("A", path.c_str()); };

		std::future<Geometry> future = thread_manager.add(mycall_vertex);

		objs.push_back(std::move(future));
	}

	Geometry square_geo = objs[1].get();
	Geometry cube_geo = objs[0].get();

	resourceM.createBuffersWithGeometry(square_geo, "SquareVertices", "SquareIndices");
	resourceM.createBuffersWithGeometry(cube_geo, "CubeVertices", "CubeIndices");


	unsigned n_obj = 1000;
	
	Texture laboon(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte);
	unsigned laboon_handle = laboon.LoadTexture("../assets/wall.jpg");

	//Cubes

		//Cube 1
	Vec3 tr_pos(0.0f, 0.0f, -200.0f);
	Vec3 obj_rot(0.0f, 0.0f, 0.0f);
	Vec3 obj_size(100.0f, 100.0f, 5.0f);

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.create_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 1", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle);
	
		//Cube 2
	tr_pos = Vec3(25.0f, 55.0f, -150.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(18.0f, 18.0f, 18.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 2", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle);

		//Cube 3
	tr_pos = Vec3(-10.0f, 2.5f, -140.5f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(20.0f, 10.0f, 20.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 3", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle);

	//Floor
	tr_pos = Vec3(0.0f, -3.0f, -250.0f);
	obj_rot = Vec3(3.14f/2.0f, 0.0f, 0.0f);
	obj_size = Vec3(200.0f, 100.0f, 200.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Wall", "SquareVertices", "SquareIndices", simpleProgram, laboon_handle);

  //Light
	size_t light_entity[2];
		
		//Directional
	tr_pos = Vec3(0.0f, 0.0f, 80.0f);

	light_entity[0] = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity[0]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[0]);

	init_directional_light_system(*ambient_light, Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);

		//Ambient
	light_entity[1] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[1]);
	tr_transform = component_manager.create_component<TransformComponent>(light_entity[1]);

	init_ambient_light_system(*ambient_light, Vec3(0.5f, 0.5f, 0.5f));
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