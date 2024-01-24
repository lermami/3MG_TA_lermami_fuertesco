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

std::vector<Vertex> LoadObjVertices(const char* path) {
	std::vector<Vertex> ret;

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

		for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
			Vertex vx;
			vx.x_ = attrib.vertices[3 * i + 0];
			vx.y_ = attrib.vertices[3 * i + 1];
			vx.z_ = attrib.vertices[3 * i + 2];

			vx.u_ = attrib.texcoords[2 * i + 0];
			vx.v_ = attrib.texcoords[2 * i + 1];

			ret.push_back(vx);
		}
	}

	return ret;
}

std::vector<unsigned> LoadObjIndices(const char* path) {
	std::vector<unsigned> ret;

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

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				ret.push_back(static_cast<unsigned int>(idx.vertex_index));
			}
			index_offset += fv;

		}

	}

	return ret;
}

int main(int, char**) {
	Engine e;
	ThreadManager thread_manager;
	ComponentManager component_manager;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

	auto simpleProgram = CreateProgram("../assets/test_shader/test.vs", "../assets/test_shader/test.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<std::vector<Vertex>>> objs_vertex;
	std::vector<std::future<std::vector<unsigned>>> objs_indices;
	obj_paths.emplace_back("../assets/Suzanne.obj");
	obj_paths.emplace_back("../assets/wolf/Wolf_obj.obj");
	obj_paths.emplace_back("../assets/gun/Gun.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<std::vector<Vertex>()> mycall_vertex = [path]() { return LoadObjVertices(path.c_str()); };
		std::function<std::vector<unsigned>()> mycall_indices = [path]() { return LoadObjIndices(path.c_str()); };

		std::future<std::vector<Vertex>> future_v = thread_manager.add(mycall_vertex);
		std::future<std::vector<unsigned>> future_i = thread_manager.add(mycall_indices);

		objs_vertex.push_back(std::move(future_v));
		objs_indices.push_back(std::move(future_i));
	}

	std::vector<Vertex> suzanne_vertices = objs_vertex[0].get();
	std::vector<Vertex> wolf_vertices = objs_vertex[1].get();
	std::vector<Vertex> tank_vertices = objs_vertex[2].get();

	std::vector<unsigned> suzanne_indices = objs_indices[0].get();
	std::vector<unsigned> wolf_indices = objs_indices[1].get();
	std::vector<unsigned> tank_indices = objs_indices[2].get();

	unsigned n_obj = 1000;

	for (unsigned i = 0; i < n_obj / 3; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(0.05f, 0.05f, 0.05f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.get_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.get_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_vertex_system(*tr_render, suzanne_vertices, suzanne_indices, simpleProgram, 0);
		init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	for (unsigned i = n_obj / 3; i < 2 * n_obj / 3; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(0.1f, 0.1f, 0.1f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.get_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.get_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_vertex_system(*tr_render, wolf_vertices, wolf_indices, simpleProgram, 0);
		init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	for (unsigned i = 2 * n_obj / 3; i < n_obj; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 obj_rot(0.0f, 0.0f, 0.0f);
		Vec3 obj_size(0.1f, 0.1f, 0.1f);

		size_t new_e = component_manager.add_entity();
		auto tr_render = component_manager.get_component<RenderComponent>(new_e);
		auto tr_transform = component_manager.get_component<TransformComponent>(new_e);

		init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
		init_vertex_system(*tr_render, tank_vertices, tank_indices, simpleProgram, 0);
		init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);
	}

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done()) {
		w.calculateLastTime();

		input_map.updateInputs();

		float input_x = 0, input_y = 0;
		float rotate = 0;
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

		if (input_map.IsKeyPressed('E')) {
			rotate = -input_velocity * 2.0f;
		}

		if (input_map.IsKeyPressed('Q')) {
			rotate = input_velocity * 2.0f;
		}

		if (input_map.IsKeyDown(kKey_LeftClick)) {
			clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (input_map.IsKeyUp(kKey_LeftClick)) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<TransformComponent>(clicked_e), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}