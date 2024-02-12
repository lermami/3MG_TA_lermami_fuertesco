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

using namespace std::chrono_literals;

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

	auto simpleProgram = CreateProgram(w, "../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");
	auto simpleProgram2 = CreateProgram(w, "../assets/Shader/ShadowMap/dirlight.vs", "../assets/Shader/ShadowMap/dirlight.fs");
	auto simpleProgram3 = CreateProgram(w, "../assets/Shader/ShadowMap/depthtest.vs", "../assets/Shader/ShadowMap/depthtest.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<Geometry>> objs;
	obj_paths.emplace_back("../assets/obj_test.obj");
	obj_paths.emplace_back("../assets/square.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<Geometry()> mycall_vertex = [path]() { return Engine::LoadObj(path.c_str()); };

		std::future<Geometry> future = thread_manager.add(mycall_vertex);

		objs.push_back(std::move(future));
	}

	Geometry laboon_geo = objs[0].get();
	Geometry square_geo = objs[1].get();

	unsigned n_obj = 1000;
	
	Vec3 tr_pos(0.0f, 1.5f, -3.0f);
	Vec3 obj_rot(0.0f, 1.57f, 0.0f);
	Vec3 obj_size(0.5f, 0.5f, 0.5f);

	Texture laboon(TextureType::kTexture_2D, TextureFormat::kRGBA);
	unsigned laboon_handle = laboon.LoadTexture("../assets/laboon/laboon.png");

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.create_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, laboon_geo, simpleProgram, laboon_handle);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	tr_pos = Vec3(0.0f, -3.0f, -110.0f);
	obj_rot = Vec3(1.57f, 0.0f, 0.0f);
	obj_size = Vec3(200.0f, 100.0f, 200.0f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, square_geo, simpleProgram3, laboon_handle);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	tr_pos = Vec3(0.5f, 1.5f, -8.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(0.5f, 0.5f, 0.5f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, laboon_geo, simpleProgram, laboon_handle);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	tr_pos = Vec3(-1.0f, 0.0f, -5.0f);
	obj_rot = Vec3(0.0f, 0.0f, 0.0f);
	obj_size = Vec3(0.25f, 0.25f, 0.25f);

	new_e = component_manager.add_entity();
	tr_render = component_manager.create_component<RenderComponent>(new_e);
	tr_transform = component_manager.create_component<TransformComponent>(new_e);

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_render_component_system(*tr_render, laboon_geo, simpleProgram, laboon_handle);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	/*
  //Light
	size_t light_entity;
	light_entity = component_manager.add_entity();
	auto ambient_light = component_manager.create_component<LightComponent>(light_entity);
	init_directional_light_system(*ambient_light, Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));*/
  
  //Camera
	size_t main_camera = component_manager.add_entity();
	auto camera_comp = component_manager.create_component<CameraComponent>(main_camera);
	w.setCurrentCam(main_camera);
	//Shadow
		//Creat depth map buffer
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

		//Create 2D Texture as the framebuffer's depth buffer
	const unsigned int shadow_w = 1024, shadow_h = 1024;
	const unsigned int scr_w = 1024, scr_h = 768;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadow_w, shadow_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//attach the framebuffer's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		imgui_transform_system(*component_manager.get_component<TransformComponent>(1));
		imgui_transform_system(*component_manager.get_component<TransformComponent>(3));
		imgui_transform_system(*component_manager.get_component<TransformComponent>(4));


		// 1. first render to depth map
		glViewport(0, 0, shadow_w, shadow_h);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		w.renderShadowMap(depthMap, simpleProgram2);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. then render scene as normal with shadow mapping (using depth map)
		glViewport(0, 0, scr_w, scr_h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		w.render(depthMap);

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}