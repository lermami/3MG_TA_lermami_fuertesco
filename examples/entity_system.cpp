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

void LoadObj(const char* path, std::vector<Vertex>& vertex, std::vector<unsigned>& indices) {
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

		for (int i = 0; i < attrib.vertices.size()/3; i++) {
			Vertex vx;
			vx.x_ = attrib.vertices[3 * i + 0];
			vx.y_ = attrib.vertices[3 * i + 1];
			vx.z_ = attrib.vertices[3 * i + 2];
			vertex.push_back(vx);
			/*
			vx.nx_ = attrib.normals[3 * (size_t)idx.normal_index + 0];
			vx.ny_ = attrib.normals[3 * (size_t)idx.normal_index + 1];
			vx.nz_ = attrib.normals[3 * (size_t)idx.normal_index + 2];
			vx.u_ = attrib.texcoords[2 * (size_t)idx.texcoord_index + 0];
			vx.v_ = attrib.texcoords[2 * (size_t)idx.texcoord_index + 1];
			*/

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

//TODO: position, rotation and size move to init_transform_component
void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v, 
												std::vector<unsigned>& indices_, unsigned int program) {

	for (int i = 0; i < v.size(); i++) {
		render.vertex_.push_back(v[i]);
	}

	render.elements_buffer_ = std::make_shared<Buffer>();
	render.elements_buffer_.get()->init((unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.elements_buffer_.get()->uploadData(&render.vertex_[0], (unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	
	render.order_buffer_ = std::make_shared<Buffer>();
	render.order_buffer_.get()->init(indices_.size());
	render.order_buffer_.get()->uploadData(&indices_[0], (unsigned)(indices_.size() * sizeof(unsigned)));

	/*
	glGenVertexArrays(1, &render.VAO_);
	glBindVertexArray(render.VAO_);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render.vertex_[0]), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	render.program_ = program;
}

void init_transform_system(TransformComponent& transfrom, Vec3& pos, Vec3& rot, Vec3& size) {
	transfrom.pos_ = pos;
	transfrom.size_ = size;
	transfrom.rot_ = rot;
}

void init_color_system(RenderComponent& render, float r, float g, float b, float a) {
	for (auto& v : render.vertex_) {
		v.r_ = r;
		v.g_ = g;
		v.b_ = b;
		v.a_ = a;
	}

}

void move_system(std::vector<std::optional<TransformComponent>>& transfroms, Vec3 mov) {

	auto r = transfroms.begin();

	for (; r != transfroms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.pos_ += mov;
	}

}

void rotate_system(std::vector<std::optional<TransformComponent>>& transfroms, Vec3 rot) {
	auto r = transfroms.begin();

	for (; r != transfroms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.rot_ += rot;
	}
}

size_t on_click_system(std::vector<std::optional<TransformComponent>>& transfroms, float mouse_x, float mouse_y) {

	mouse_x = (mouse_x / 1024 * 2) - 1;
	mouse_y = ((mouse_y / 768 * 2) - 1) * -1;

	auto r = transfroms.begin();
	size_t e = 0;

	for (; r != transfroms.end(); r++, e++) {
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

void set_position_system(TransformComponent& transform, Vec3 pos) {

	transform.pos_ = pos;
}

void render_system(std::vector<std::optional<RenderComponent>>& renders, std::vector<std::optional<TransformComponent>>& trasforms) {

	auto r = renders.begin();
	auto t = trasforms.begin();

	for (; r != renders.end(); r++ , t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		Mat4 m = transform.model_matrix_;

		m = m.Identity();
		m = m.Multiply(m.Translate(transform.pos_));
		m = m.Multiply(m.RotateX(transform.rot_.x).Multiply(m.RotateY(transform.rot_.y).Multiply(m.RotateZ(transform.rot_.z))));
		m = m.Multiply(m.Scale(transform.size_));
		m = m.Transpose();

		glUseProgram(render.program_);
		GLint modelMatrixLoc = glGetUniformLocation(render.program_, "u_m_matrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

		render.elements_buffer_.get()->bind(kTarget_VertexData);
		render.elements_buffer_.get()->uploadFloatAttribute(0, 3, sizeof(render.vertex_[0]), (void*)0);
		render.elements_buffer_.get()->uploadFloatAttribute(1, 4, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));

		auto order_buffer = render.order_buffer_.get();

		order_buffer->bind(kTarget_Elements);
		glDrawElements(GL_TRIANGLES, order_buffer->size(), GL_UNSIGNED_INT, 0);
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

	auto simpleProgram = CreateProgram("../include/test.vs", "../include/test.fs");

	std::vector<Vertex> triangle = {
		{-0.05f, -0.05f, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
		{0.05f, -0.05f, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		{0.0f, 0.05f, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
	};

	std::vector<unsigned> tr_indices = { 0, 1, 2 };

	//Create n triangles in random position
	for (int i = 0; i < n_entities; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 tr_size(2.0f, 2.0f, 2.0f);
		Vec3 tr_rot(0.0f, 0.0f, 0.0f);

		entities.push_back(component_manager.add_entity());
		auto tr_render = component_manager.get_component<RenderComponent>(entities[i]);
		auto tr_transform = component_manager.get_component<TransformComponent>(entities[i]);
		init_transform_system(*tr_transform, tr_pos, tr_rot, tr_size);
		init_vertex_system(*tr_render, triangle, tr_indices, simpleProgram);
	}

	//Create obj entity
	std::vector<Vertex> obj_test;
	std::vector<unsigned> obj_indices_test;
	LoadObj("../include/Suzanne.obj", obj_test, obj_indices_test);
	Vec3 obj_pos(0.0f, 0.0f, 0.0f);
	Vec3 obj_rot(0.0f, 0.0f, 0.0f);
	Vec3 obj_size(0.5f, 0.5f, 0.5f);
	entities.push_back(component_manager.add_entity());
	auto tr_render = component_manager.get_component<RenderComponent>(entities.back());
	auto tr_transform = component_manager.get_component<TransformComponent>(entities.back());

	init_transform_system(*tr_transform, obj_pos, obj_rot, obj_size);
	init_vertex_system(*tr_render, obj_test, obj_indices_test, simpleProgram);
	init_color_system(*tr_render, 0.5f, 0.0f, 0.5f, 1.0f);

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
			clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (mouse_right_click.IsKeyDown()) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<TransformComponent>(entities[clicked_e]), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));
		render_system(*component_manager.get_component_list<RenderComponent>(), *component_manager.get_component_list<TransformComponent>());

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}