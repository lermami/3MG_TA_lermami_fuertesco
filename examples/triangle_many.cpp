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

#include "component_manager.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
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

	std::vector<size_t> entities;
	auto simpleProgram = CreateProgram("../include/test.vs", "../include/test.fs");
	
	//Create n triangles in random position
	int n_triangles = 1000;

	std::vector<Vertex> triangle = {
		{-1.0f, -1.0f, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
		{1.0f, -1.0f, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		{0.0f, 1.0f, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
	};

	std::vector<unsigned> tr_indices = { 0, 1, 2 };

	for (int i = 0; i < n_triangles; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 tr_size(0.1f, 0.1f, 0.0f);
		Vec3 tr_rot(0.0f, 0.0f, 0.0f);

		entities.push_back(component_manager.add_entity());
		auto tr_render = component_manager.get_component<RenderComponent>(entities[i]);
		auto tr_transform = component_manager.get_component<TransformComponent>(entities[i]);
		init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
		init_vertex_system(*tr_render, triangle, tr_indices, simpleProgram);
	}

	//Input Declaration
	Input input_map(w);
	float input_velocity = 0.05f;

	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done()) {
		w.calculateLastTime();
		glClear(GL_COLOR_BUFFER_BIT);

		input_map.updateInputs();

		float input_x = 0, input_y = 0;
		float rotate = 0;
		double mouse_x = 0, mouse_y = 0;

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

		if (input_map.IsKeyPressed('E')) {
			rotate = -input_velocity;
		}

		if (input_map.IsKeyPressed('Q')) {
			rotate = input_velocity;
		}
		
		if (input_map.IsKeyDown(kKey_LeftClick)) {
			clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (input_map.IsKeyUp(kKey_LeftClick)) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<TransformComponent>(entities[clicked_e-1]), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}