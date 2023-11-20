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

#include "matrix_4.hpp"

struct ComponentLife {};

std::vector<Vertex> LoadObj(const char* path) {
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
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex vertex;
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				vertex.x_ = attrib.vertices[3 * (size_t)idx.vertex_index + 0];
				vertex.y_ = attrib.vertices[3 * (size_t)idx.vertex_index + 1];
				vertex.z_ = attrib.vertices[3 * (size_t)idx.vertex_index + 2];
				vertex.nx_ = attrib.normals[3 * (size_t)idx.normal_index + 0];
				vertex.ny_ = attrib.normals[3 * (size_t)idx.normal_index + 1];
				vertex.nz_ = attrib.normals[3 * (size_t)idx.normal_index + 2];
				vertex.u_ = attrib.texcoords[2 * (size_t)idx.texcoord_index + 0];
				vertex.v_ = attrib.texcoords[2 * (size_t)idx.texcoord_index + 1];

				ret.push_back(vertex);
			}
			index_offset += fv;

			// per-face material
			//shapes[s].mesh.material_ids[f];
		}
	}

	return ret;
}

void physics_system(std::vector<std::optional<Position>>& positions, std::vector<std::optional<Physics>>& physics) {

	auto p = positions.begin();
	auto ph = physics.begin();
	for (; p != positions.end(); p++, ph++) {
		if (!p->has_value() || !ph->has_value()) continue;
		auto& pv = p->value();
		auto& phv = ph->value();

		// Fisica dura

	}
	assert(ph == physics.end());
}

void initialize_position_system(Position& pos, float x, float y) {
	pos.x = x;
	pos.y = y;
}

void print_position_system(size_t n, Position& pos) {
	printf("Entity %zu: X: %f Y: %f\n", n, pos.x, pos.y);
}

void init_render_system(RenderComponent& render) {

}

void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v, Vec3& pos, Vec3& rot, Vec3& size, unsigned int program) {
	for (int i = 0; i < v.size(); i++) {
		render.vertex_.push_back(v[i]);
		render.transformed_vertex_.push_back(v[i]);
	}

	render.pos_ = pos;
	render.size_ = size;
	render.rot_ = rot;

	render.buffer_.init((unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.buffer_.uploadData(&render.vertex_[0], (unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.program_ = program;
}

void move_system(std::vector<std::optional<RenderComponent>>& renders, Vec3 mov) {

	auto r = renders.begin();

	for (; r != renders.end(); r++) {
		if (!r->has_value()) continue;
		auto& render = r->value();
		render.pos_ += mov;

		if (render.pos_.x > 1) {
			render.pos_.x = -1;
		}

		if (render.pos_.x < -1) {
			render.pos_.x = 1;
		}

		if (render.pos_.y > 1) {
			render.pos_.y = -1;
		}

		if (render.pos_.y < -1) {
			render.pos_.y = 1;
		}
	}

}

void rotate_system(std::vector<std::optional<RenderComponent>>& renders, Vec3 rot) {
	auto r = renders.begin();

	for (; r != renders.end(); r++) {
		if (!r->has_value()) continue;
		auto& render = r->value();
		render.rot_ += rot;
	}
}

size_t on_click_system(std::vector<std::optional<RenderComponent>>& renders, float mouse_x, float mouse_y) {

	mouse_x = (mouse_x / 1024 * 2) - 1;
	mouse_y = ((mouse_y / 768 * 2) - 1) * -1;

	auto r = renders.begin();
	size_t e = 0;

	for (; r != renders.end(); r++, e++) {
		if (!r->has_value()) continue;

		//TODO: Add size
		float x1 = r->value().pos_.x;
		float x2 = r->value().pos_.x;
		float y1 = r->value().pos_.y;
		float y2 = r->value().pos_.y;

		if (mouse_x > x1 && mouse_x < x2 &&
			mouse_y < y1 && mouse_y > y2)
			return e;

	}

	return 0;
}

void set_position_system(RenderComponent& render, Vec3 pos) {

	render.pos_ = pos;
}

void render_system(std::vector<std::optional<RenderComponent>>& renders) {

	auto r = renders.begin();

	glUseProgram(r->value().program_);
	GLint modelMatrixLoc = glGetUniformLocation(r->value().program_, "u_m_matrix");

	for (; r != renders.end(); r++) {
		if (!r->has_value()) continue;
		auto& render = r->value();


		Mat4 m = m.Identity();
		m = m.Multiply(m.Translate(render.pos_));
		m = m.Multiply(m.RotateX(render.rot_.x).Multiply(m.RotateY(render.rot_.y).Multiply(m.RotateZ(render.rot_.z))));
		m = m.Multiply(m.Scale(render.size_));

		for (auto i = 0; i < render.transformed_vertex_.size(); i++) {
			Vec3 transformed_pos(render.transformed_vertex_[i].x_, render.transformed_vertex_[i].y_, render.transformed_vertex_[i].z_);
			Vec3 pos(render.vertex_[i].x_, render.vertex_[i].y_, render.vertex_[i].z_);

			transformed_pos = m.Transform_Mat4_Vec3(m, pos);

			render.transformed_vertex_[i].x_ = transformed_pos.x;
			render.transformed_vertex_[i].y_ = transformed_pos.y;
			render.transformed_vertex_[i].z_ = transformed_pos.z;
		}

		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

		render.buffer_.uploadFloatAttribute(0, 3, sizeof(render.transformed_vertex_[0]), (void*)0);
		render.buffer_.uploadFloatAttribute(1, 3, sizeof(render.transformed_vertex_[0]), (void*)(3 * sizeof(float)));

		//TODO: acabar el draw y crear el buffer de indices
		//gldrawElements
	}
}

int main(int, char**) {
	Engine e;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

	if (glewInit() != GLEW_OK) return -1;

	ComponentManager component_manager;

	std::vector<size_t> entities;
	int n_entities = 100;

	std::vector<Vertex> obj_test = LoadObj("../include/obj_test.obj");

	std::string v = ReadFiles("../include/test.vs");
	std::string f = ReadFiles("../include/test.fs");

	auto simpleVertex = CreateShader(0);
	CompileShader(simpleVertex, v.c_str());
	auto simpleFragment = CreateShader(1);
	CompileShader(simpleFragment, f.c_str());
	auto simpleProgram = CreateProgram(simpleVertex, simpleFragment);

	//Create obj entity
#if 0
	Vec3 obj_pos(0.0f, 0.0f, 0.0f);
	Vec3 obj_rot(0.0f, 0.0f, 0.0f);
	Vec3 obj_size(1.0f, 1.0f, 1.0f);
	entities.push_back(component_manager.add_entity());
	auto tr_render = component_manager.get_component<RenderComponent>(entities[0]);
	init_vertex_system(*tr_render, obj_test, obj_pos, obj_rot, obj_size, simpleProgram);
#else
	
	std::vector<Vertex> triangle = {
		{-0.05f, -0.05f, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0.05f, -0.05f, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{0.0f, 0.05f, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	};

	//Create n triangles in random position
	for (int i = 0; i < n_entities; i++) {
		Vec3 tr_pos;

		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0;

		Vec3 tr_size(2.0f, 2.0f, 2.0f);
		Vec3 tr_rot(0.0f, 0.0f, 0.0f);

		entities.push_back(component_manager.add_entity());
		auto tr_render = component_manager.get_component<RenderComponent>(entities[i]);
		init_vertex_system(*tr_render, triangle, tr_pos, tr_rot, tr_size, simpleProgram);
	}
	
#endif
	//Input Declaration
	InputMap input_map(w);
	float input_velocity = 0.05f;

	Input up(input_map, kKey_W);
	Input down(input_map, kKey_S);
	Input left(input_map, kKey_A);
	Input right(input_map, kKey_D);
	Input rotate_r(input_map, kKey_Q);
	Input rotate_l(input_map, kKey_E);
	Input mouse_left_click(input_map, kKey_LeftClick);
	Input mouse_right_click(input_map, kKey_RightClick);

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

		if (up.IsKeyPressed()) {
			input_y = input_velocity;
		}

		if (down.IsKeyPressed()) {
			input_y = -input_velocity;
		}

		if (left.IsKeyPressed()) {
			input_x = -input_velocity;
		}

		if (right.IsKeyPressed()) {
			input_x = input_velocity;
		}

		if (rotate_l.IsKeyPressed()) {
			rotate = -input_velocity;
		}

		if (rotate_r.IsKeyPressed()) {
			rotate = input_velocity;
		}

		if (mouse_left_click.IsKeyDown()) {
			clicked_e = on_click_system(*component_manager.get_component_list<RenderComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (mouse_right_click.IsKeyDown()) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<RenderComponent>(entities[clicked_e]), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<RenderComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<RenderComponent>(), Vec3(0.0f, 0.0f, rotate));
		render_system(*component_manager.get_component_list<RenderComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}