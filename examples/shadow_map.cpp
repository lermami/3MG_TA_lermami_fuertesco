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
	Vec3 tr_pos(-6.0f, 6.0f, -20.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(2.0f, 10.0f, 10.0f);

	size_t new_e = component_manager.add_entity(TransformComponent(Vec3(-6.0f, 6.0f, -20.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(2.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 1", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle));
	
	tr_pos = Vec3(0.25f, 5.5f, -5.75f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.8f, 1.8f, 1.8f);

	new_e = component_manager.add_entity(TransformComponent(Vec3(0.25f, 5.5f, -5.75f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.8f, 1.8f, 1.8f)),
		RenderComponent("Cube 2", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle));

	tr_pos = Vec3(-11.5f, 0.25f, -8.75f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(3.0f, 3.0f, 3.0f);

	new_e = component_manager.add_entity(TransformComponent(Vec3(-11.5f, 0.25f, -8.75f), Vec3(0.0f, 0.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f)),
		RenderComponent("Cube 3", "CubeVertices", "CubeIndices", simpleProgram, laboon_handle));

	//Floor
	tr_pos = Vec3(0.0f, -3.0f, -110.0f);
	obj_rot = Vec3(3.14f/2.0f, 0.0f, 0.0f);
	obj_size = Vec3(200.0f, 100.0f, 200.0f);

	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -3.0f, -110.0f), Vec3(3.14f / 2.0f, 0.0f, 0.0f), Vec3(200.0f, 100.0f, 200.0f)),
		RenderComponent("Wall", "SquareVertices", "SquareIndices", simpleProgram, laboon_handle));

  //Light
	size_t light_entity[2];
		
		//Directional
	light_entity[0] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, -80.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)));

		//Ambient
	light_entity[1] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, -80.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f)));
  
  //Camera
	size_t main_camera = component_manager.add_entity(CameraComponent());
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