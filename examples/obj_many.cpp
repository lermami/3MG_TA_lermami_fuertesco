#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tiny_obj_loader.h>

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

bool LoadObj(const char* path, std::vector<Vertex>& vertex, std::vector<unsigned>& indices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string warning, error;

	bool err = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);

	if (!err) {
		if (!error.empty()) {
			std::cout << "Error loading obj: " << error.c_str();
		}
	}

	if (!warning.empty()) {
		std::cout << "Warning loading obj: " << warning.c_str();
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;

		for (int i = 0; i < attrib.vertices.size() / 3; i++) {
			Vertex vx;
			vx.x_ = attrib.vertices[3 * i + 0];
			vx.y_ = attrib.vertices[3 * i + 1];
			vx.z_ = attrib.vertices[3 * i + 2];
			vertex.push_back(vx);
		}

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				indices.push_back(static_cast<unsigned int>(idx.vertex_index));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}

	}

	return true;
}


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

	//Create obj entity
	std::vector<Vertex> obj_test;
	std::vector<unsigned> obj_indices_test;
	std::future<bool> future;

	std::function<bool()> mycall_double = [&obj_test, &obj_indices_test]() { return LoadObj("../include/Suzanne.obj", obj_test, obj_indices_test); };
	future = thread_manager.add(mycall_double);

	thread_manager.waitFuture(future);

	unsigned n_obj = 533;

	for (unsigned i = 0; i < n_obj; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(0.05f, 0.05f, 0.05f);

		entities.push_back(component_manager.add_entity());
		auto tr_render = component_manager.get_component<RenderComponent>(entities[i]);
		auto tr_transform = component_manager.get_component<TransformComponent>(entities[i]);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_vertex_system(*tr_render, obj_test, obj_indices_test, simpleProgram);
		init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
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
			set_position_system(*component_manager.get_component<TransformComponent>(entities[clicked_e - 1]), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}