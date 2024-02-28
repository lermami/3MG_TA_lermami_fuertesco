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
#include "Renderer.hpp"
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

	auto texture_shader = CreateProgram(w, "../assets/BasicShader/Texture/Texture.vs", "../assets/BasicShader/Texture/Texture.fs");
	auto texture_light_shader = CreateProgram(w, "../assets/BasicShader/Texture/TextureLight.vs", "../assets/BasicShader/Texture/TextureLight.fs");
	auto texture_light_shadow_shader = CreateProgram(w, "../assets/BasicShader/Texture/TextureLightShadow.vs", "../assets/BasicShader/Texture/TextureLightshadow.fs");

	auto color_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/Color.vs", "../assets/BasicShader/BasicColor/Color.fs");
	auto color_light_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/ColorLight.vs", "../assets/BasicShader/BasicColor/ColorLight.fs");
	auto color_light_shadow_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/ColorLightShadow.vs", "../assets/BasicShader/BasicColor/ColorLightshadow.fs");


	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/obj_test.obj");
	obj_paths.emplace_back("../assets/square.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> mycall_vertex = [&]() { return resourceM.LoadObj("A", path.c_str()); };

		std::future<Geometry> future = thread_manager.add(mycall_vertex);

		objs.push_back(std::move(future));
	}

	Geometry cube_geo = objs[0].get();

	resourceM.createBuffersWithGeometry(cube_geo, "CubeVertices", "CubeIndices");

	unsigned n_obj = 1000;

	Texture wall(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte);
	unsigned wall_handle = wall.LoadTexture("../assets/wall.jpg");

	//Cubes
		//1
	Vec3 tr_pos(-3.0f, 2.0f, -8.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(1.0f, 1.0f, 1.0f);

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.create_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 1", "CubeVertices", "CubeIndices", texture_shader, wall_handle);

		//2
	tr_pos = Vec3(0.0f, 2.0f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.0f, 1.0f, 1.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 2", "CubeVertices", "CubeIndices", texture_light_shader, wall_handle);

		//3
	tr_pos = Vec3(3.0f, 2.0f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.0f, 1.0f, 1.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Cube 3", "CubeVertices", "CubeIndices", texture_light_shadow_shader, wall_handle);

		//4
	tr_pos = Vec3(-3.0f, -2.0f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.0f, 1.0f, 1.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);
	auto color = component_manager.create_component<ColorComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_color_system(*color, 0.5f, 0.5f, 0.75f, 1.0f);
	init_render_component_system(*tr_render, "Cube 3", "CubeVertices", "CubeIndices", color_shader, 0);

		//5
	tr_pos = Vec3(0.0f, -2.0f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.0f, 1.0f, 1.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);
	color = component_manager.create_component<ColorComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_color_system(*color, 0.5f, 0.5f, 0.75f, 1.0f);
	init_render_component_system(*tr_render, "Cube 4", "CubeVertices", "CubeIndices", color_light_shader, 0);

		//6
	tr_pos = Vec3(3.0f, -2.0f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(1.0f, 1.0f, 1.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);
	color = component_manager.create_component<ColorComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_color_system(*color, 0.5f, 0.5f, 0.75f, 1.0f);
	init_render_component_system(*tr_render, "Cube 5", "CubeVertices", "CubeIndices", color_light_shadow_shader, 0);

	//Back Wall
	tr_pos = Vec3(0.0f, 0.0f, -12.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(5.0f, 5.0f, 0.5f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, "Wall", "CubeVertices", "CubeIndices", texture_light_shadow_shader, wall_handle);

	//Light
	size_t light_entity[2];

	light_entity[0] = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity[0]);
	init_directional_light_system(*ambient_light, Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));

	light_entity[1] = component_manager.add_entity();
	ambient_light = component_manager.create_component<LightComponent>(light_entity[1]);
	init_ambient_light_system(*ambient_light, Vec3(0.5f, 0.5f, 0.5f));

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

		imgui_transform_system(e, w);

		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}