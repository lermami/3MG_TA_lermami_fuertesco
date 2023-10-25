#include<vector>
#include<optional>
#include<cassert>
#include<memory>
#include<tuple>
#include<unordered_map>
#include<typeinfo>

struct Position { };

struct Physics {
	float acceleration;
};

struct Material {};

struct component_base {

};

template<typename T>
struct component_list : component_base {
	std::vector < std::optional<T>> components_;
};

struct ComponentManager {

	std::unordered_map<std::size_t, std::unique_ptr<component_base>> components_;

	ComponentManager() {
		add_component_class<Position>();
		add_component_class<Physics>();
		add_component_class<Material>();
	}
	template<typename T> void add_component_class() {
		components_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
	}

	unsigned int new_entity();

	// Previous code
	//	std::vector<std::unique_ptr<component_base>> components_;

	//std::tuple<Position*,Physics*,Material*> get_entity(unsigned int e);
	//unsigned int new_entity(std::optional<Position> p = std::nullopt, std::optional<Physics> ph = std::nullopt, std::optional<Material> m = std::nullopt);
};

// Previous code
//std::tuple<Position*, Physics*, Material*> ComponentManager::get_entity(unsigned int e) {
//	assert(e);
//	e = e - 1;
//	return { position[e].has_value() ? &position[e].value() : nullptr,
//		physics[e].has_value() ? &physics[e].value() : nullptr,
//		material[e].has_value() ? &material[e].value() : nullptr };
//}

unsigned int ComponentManager::new_entity() {
	auto p = components_.begin();
	
	for (; p != components_.end(); p++) {
		p->second.get();
	}
	
	//return position.size();
	return 0;
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

unsigned int entity;

struct ComponentLife {};

int main(int, char**) {
	ComponentManager component_manager;

	component_manager.add_component_class<ComponentLife>();

	// Previous code
	// auto player = component_manager.new_entity({}, {}, {});

	auto player = component_manager.new_entity();


	std::vector<unsigned int> obstacles;
	for (int i = 0; i != 30; i++) {
		// Previous code
		//obstacles.push_back(component_manager.new_entity({}, {}, {}));
	}
	while (true) {
		//if (input.acceleration) {
			// Previous code
			//auto [_, ph, ___] = component_manager.get_entity(player);
			//ph->acceleration += 1;
		//}
		// Previous code
//		physics_system(component_manager.position, component_manager.physics);

	}
	return 0;
}