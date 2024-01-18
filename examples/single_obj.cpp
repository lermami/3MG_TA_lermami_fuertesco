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
using namespace std::chrono_literals;

#include "matrix_4.hpp"

std::vector<Vertex> LoadObjVertices(const char* path) {
	std::vector<Vertex> ret;
	std::vector<unsigned> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string warning, error;

	bool err = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);
	int id = 0;

	if (!err) {
		if (!error.empty()) {
			std::cout << "Error loading obj: " << error.c_str();
		}
	}
	/*
	if (!warning.empty()) {
		std::cout << "Warning loading obj: " << warning.c_str();
	}*/

	std::map<int, Vertex> map;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex vertex;
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				vertex.pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				vertex.pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				vertex.pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					vertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					vertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					vertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					vertex.uv.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.uv.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// vertex.red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// vertex.green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// vertex.blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

				bool found = false;

				for (auto it = map.begin(); it != map.end() && !found; it++) {
					if (vertex == it->second) {
						indices.push_back(it->first);
						found = true;
					}
				}
				
				if (!found) {
					map.emplace(id, vertex);
					ret.push_back(vertex);
					indices.push_back(id);
					id++;
				}
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return ret;
}

std::vector<unsigned> LoadObjIndices(const char* path) {
	std::vector<Vertex> ret;
	std::vector<unsigned> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string warning, error;

	bool err = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);
	int id = 0;

	if (!err) {
		if (!error.empty()) {
			std::cout << "Error loading obj: " << error.c_str();
		}
	}
	/*
	if (!warning.empty()) {
		std::cout << "Warning loading obj: " << warning.c_str();
	}*/

	std::map<int, Vertex> map;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex vertex;
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				vertex.pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				vertex.pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				vertex.pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					vertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					vertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					vertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					vertex.uv.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.uv.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// vertex.red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// vertex.green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// vertex.blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

				bool found = false;

				for (auto it = map.begin(); it != map.end() && !found; it++) {
					if (vertex == it->second) {
						indices.push_back(it->first);
						found = true;
					}
				}

				if (!found) {
					map.emplace(id, vertex);
					ret.push_back(vertex);
					indices.push_back(id);
					id++;
				}
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return indices;
}

int main(int, char**) {
	Engine e;
	ThreadManager thread_manager;
	ComponentManager component_manager;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.initImGui();

	auto simpleProgram = CreateProgram("../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");

	std::vector<std::string> obj_paths;
	std::vector<std::future<std::vector<Vertex>>> objs_vertex;
	std::vector<std::future<std::vector<unsigned>>> objs_indices;
	obj_paths.emplace_back("../assets/laboon/laboon.obj");

	//Create obj entity
	for (auto& path : obj_paths) {
		std::function<std::vector<Vertex>()> mycall_vertex = [path]() { return LoadObjVertices(path.c_str()); };
		std::function<std::vector<unsigned>()> mycall_indices = [path]() { return LoadObjIndices(path.c_str()); };

		std::future<std::vector<Vertex>> future_v = thread_manager.add(mycall_vertex);
		std::future<std::vector<unsigned>> future_i = thread_manager.add(mycall_indices);

		objs_vertex.push_back(std::move(future_v));
		objs_indices.push_back(std::move(future_i));
	}

	std::vector<Vertex> laboon_vertices = objs_vertex[0].get();
	std::vector<unsigned> laboon_indices = objs_indices[0].get();

	unsigned n_obj = 1000;
	
	Vec3 tr_pos(0.0f, 0.0f, 0.0f);
	Vec3 obj_rot(0.0f, 0.0f, 0.0f);
	Vec3 obj_size(0.007f, 0.007f, 0.007f);

	size_t new_e = component_manager.add_entity();
	auto tr_render = component_manager.get_component<RenderComponent>(new_e);
	auto tr_transform = component_manager.get_component<TransformComponent>(new_e);

	Texture laboon(TextureType::kTexture_2D, TextureFormat::kRGBA);
	unsigned laboon_tex = laboon.LoadTexture("../assets/laboon/laboon.png");

	init_transform_system(*tr_transform, tr_pos, obj_rot, obj_size);
	init_vertex_system(*tr_render, laboon_vertices, laboon_indices, simpleProgram, laboon_tex);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);

	while (!w.is_done()) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		float input_x = 0, input_y = 0;
		float rotate = 1;
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
		
		imgui_transform_system(*component_manager.get_component<TransformComponent>(new_e));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		//rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(rotate * w.getDeltaTime(), rotate * w.getDeltaTime(), 0.0f));
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}