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

void initialize_position_system(Position& pos) {
	pos.x = rand() % 100;
	pos.y = rand() % 100;
}

void print_position_system(size_t n, Position& pos) {
	printf("Entity %d: %d %d\n", n, pos.x, pos.y);
	pos.x = rand() % 100;
	pos.y = rand() % 100;
}

void init_render_system(RenderComponent& render) {

}

void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v, Position& pos, unsigned int program) {
	for (int i = 0; i < v.size(); i++) {
		render.vertex_.push_back(v[i]);
	}

	render.pos_ = pos;
	render.buffer_.init(sizeof(render.vertex_[0]) * render.vertex_.size());
	render.buffer_.bind(Target::kTarget_Elements);
	render.program_ = program;
}

void render_system(RenderComponent& render) {
	glUseProgram(render.program_);
	render.buffer_.uploadData(&render.vertex_[0], sizeof(render.vertex_[0]) * render.vertex_.size());
	render.buffer_.uploadFloatAttribute(0, 3, sizeof(render.vertex_[0]), (void*)0);
	render.buffer_.uploadFloatAttribute(1, 3, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int, char**) {
	Engine e;

	auto maybe_w = Window::create(e, 1024, 768, "Test Window");
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.init(0.4, 0.4, 0.4, 1);

	if (glewInit() != GLEW_OK) return -1;

	ComponentManager component_manager;

	std::vector<size_t> entities;
	int n_entities = 100;

	std::string v = ReadFiles("../include/test.vs");
	std::string f = ReadFiles("../include/test.fs");

	auto simpleVertex = CreateShader(0);
	CompileShader(simpleVertex, v.c_str());
	auto simpleFragment = CreateShader(1);
	CompileShader(simpleFragment, f.c_str());
	auto simpleProgram = CreateProgram(simpleVertex, simpleFragment);

	for (int i = 0; i < n_entities; i++) {
		Position tr_pos;
		tr_pos.x = (rand() % 3) - 1;
		tr_pos.y = (rand() % 3) - 1;
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

	while (!w.is_done()) {
		w.calculateLastTime();
		glClear(GL_COLOR_BUFFER_BIT);

		for(int i=0;i<n_entities;i++)
			render_system(*component_manager.get_component<RenderComponent>(entities[i]));

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

	/*
	for (int i = 0; i < n_entities; i++) {
		entities.push_back(component_manager.add_entity());
		auto entity_pos = component_manager.get_component<Position>(entities[i]);
		initialize_position_system(*entity_pos);
	}

	printf("Entities created: %d\n", n_entities);
	for (int i = 0; i < entities.size(); i++) {
		auto entity_pos = component_manager.get_component<Position>(entities[i]);
		print_position_system(entities[i], *entity_pos);
	}
	printf("\n\n");

	component_manager.remove_entity(entities[0]);
	entities.erase(entities.begin());
	component_manager.remove_entity(entities[0]);
	entities.erase(entities.begin());

	printf("Removed entities 1 and 2\n");
	for (int i = 0; i < entities.size(); i++) {
		auto entity_pos = component_manager.get_component<Position>(entities[i]);
		print_position_system(entities[i], *entity_pos);
	}
	printf("\n\n");

	size_t new_1 = component_manager.add_entity();
	size_t new_2 = component_manager.add_entity();
	entities.push_back(new_1);
	entities.push_back(new_2);
	auto entity_pos1 = component_manager.get_component<Position>(new_1);
	auto entity_pos2 = component_manager.get_component<Position>(new_2);
	initialize_position_system(*entity_pos1);
	initialize_position_system(*entity_pos2);

	printf("Added enities 1 and 2\n");
	for (int i = 0; i < entities.size(); i++) {
		auto entity_pos = component_manager.get_component<Position>(entities[i]);
		print_position_system(entities[i], *entity_pos);
	}
	printf("\n\n");

	while (true) {
		//if (input.acceleration) {
			// Previous code
			//auto [_, ph, ___] = component_manager.get_entity(player);
			//ph->acceleration += 1;
		//}
		//Previous code
    //physics_system(component_manager.position, component_manager.physics);

	}
	*/
}