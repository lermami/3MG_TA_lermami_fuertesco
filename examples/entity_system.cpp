#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>

#include "component_manager.hpp"

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

int main(int, char**) {
	ComponentManager component_manager;

	component_manager.add_component_class<ComponentLife>();

	std::vector<size_t> entities;
	int n_entities = 10;

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
	return 0;
}