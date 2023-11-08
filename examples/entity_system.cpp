#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>

#include "component_manager.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"

struct ComponentLife {};

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

void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v, Position& pos, unsigned int program) {
	for (int i = 0; i < v.size(); i++) {
		render.vertex_.push_back(v[i]);
	}

	render.pos_ = pos;
	render.buffer_.init((unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.buffer_.bind(Target::kTarget_Elements);
	render.program_ = program;
}

void move_system(RenderComponent& render, float x, float y) {

	for (auto& v : render.vertex_) {
		v.x_ += x;
		v.y_ += y;
	}
}

size_t on_click_system(std::vector<std::optional<RenderComponent>>& renders, float mouse_x, float mouse_y) {

	mouse_x = (mouse_x / 1024 * 2) - 1;
	mouse_y = ((mouse_y / 768 * 2) - 1) * -1;

	auto r = renders.begin();
	size_t e = 1;

	for (; r != renders.end(); r++, e++) {
		if (!r->has_value()) continue;
		auto& v = r->value().vertex_;

		if (mouse_x > v[0].x_ && mouse_x < v[1].x_ &&
			mouse_y < v[2].y_ && mouse_y > v[0].y_)
			return e;

	}

	return 0;
}

void set_position_system(RenderComponent& render, float x, float y) {

	x = (x / 1024 * 2) - 1;
	y = ((y / 768 * 2) - 1) * -1;

	render.vertex_[0].x_ = x - 0.05f;
	render.vertex_[0].y_ = y - 0.05f;

	render.vertex_[1].x_ = x + 0.05f;
	render.vertex_[1].y_ = y - 0.05f;

	render.vertex_[2].x_ = x;
	render.vertex_[2].y_ = y + 0.05f;
}

void render_system(RenderComponent& render) {

	glUseProgram(render.program_);
	render.buffer_.uploadData(&render.vertex_[0], (unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.buffer_.uploadFloatAttribute(0, 3, sizeof(render.vertex_[0]), (void*)0);
	render.buffer_.uploadFloatAttribute(1, 3, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int, char**) {
	Engine e;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.init(0.4f, 0.4f, 0.4f, 1.0f);

	if (glewInit() != GLEW_OK) return -1;

	ComponentManager component_manager;

	std::vector<size_t> entities;
	int n_entities = 5000;

	std::string v = ReadFiles("../include/test.vs");
	std::string f = ReadFiles("../include/test.fs");

	auto simpleVertex = CreateShader(0);
	CompileShader(simpleVertex, v.c_str());
	auto simpleFragment = CreateShader(1);
	CompileShader(simpleFragment, f.c_str());
	auto simpleProgram = CreateProgram(simpleVertex, simpleFragment);

	for (int i = 0; i < n_entities; i++) {
		Position tr_pos;

		tr_pos.x = (float)((rand() % 300) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 300) - 100) / 100.0f;
		tr_pos.z = 0;

		std::vector<Vertex> triangle = {
			{tr_pos.x - 0.05f, tr_pos.y - 0.05f, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
			{tr_pos.x + 0.05f, tr_pos.y - 0.05f, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
			{tr_pos.x + 0.0f, tr_pos.y + 0.05f, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		};

		entities.push_back(component_manager.add_entity());
		auto tr_render = component_manager.get_component<RenderComponent>(entities[i]);
		init_vertex_system(*tr_render, triangle, tr_pos, simpleProgram);
	}


	//Input Declaration
	InputMap input_map(w);
	float input_velocity = 0.05f;

	Input up(input_map, kKey_W);
	Input down(input_map, kKey_S);
	Input left(input_map, kKey_A);
	Input right(input_map, kKey_D);
	Input mouse_left_click(input_map, kKey_LeftClick);
	Input mouse_right_click(input_map, kKey_RightClick);

	double mouse_x = 0, mouse_y = 0;
		size_t clicked_e = 0;

	while (!w.is_done()) {
		w.calculateLastTime();
		glClear(GL_COLOR_BUFFER_BIT);

		input_map.updateInputs();


		float input_x = 0, input_y = 0;
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

		if (mouse_left_click.IsKeyDown()) {
			clicked_e = on_click_system(*component_manager.get_component_list<RenderComponent>(), mouse_x, mouse_y);
		}

		if (mouse_right_click.IsKeyDown()) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<RenderComponent>(entities[clicked_e]), (float)mouse_x, (float)mouse_y);

		for (int i = 0; i < n_entities; i++) {
			RenderComponent* aux_render = component_manager.get_component<RenderComponent>(entities[i]);
			move_system(*aux_render, input_x, input_y);

			render_system(*aux_render);
		}

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}