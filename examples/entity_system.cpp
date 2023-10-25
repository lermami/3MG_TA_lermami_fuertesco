#include<vector>
#include<optional>
#include<cassert>
#include<memory>
#include<tuple>
#include<unordered_map>
#include<typeinfo>
#include<stdexcept>

struct Position {
	int x, y;
};

struct Physics {
	float acceleration;
};

struct Material {};

struct component_base {
	virtual void add_component() = 0;
	virtual void delete_component(size_t id) = 0;
	virtual size_t size() = 0;
};

template<typename T>
struct component_list : component_base {
	std::vector < std::optional<T>> components_;

	virtual void add_component() override {
		components_.emplace_back();
	}

	virtual size_t size() override {
		return components_.size();
	}

	virtual void remove_component(size_t id) override {
		components_[id].reset();
	}
};

struct ComponentManager {

	std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_classes_;
	std::vector <size_t> deleted_components_;

	ComponentManager() {
		add_component_class<Position>();
		add_component_class<Physics>();
		add_component_class<Material>();
	}

	template<typename T> void add_component_class() {
		component_classes_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
	}

	template<typename T> T* get_component(size_t e);

	size_t add_entity() {
		if (deleted_components_.size() > 0) {
			size_t id = deleted_components_.back();
			deleted_components_.pop_back();
			return id;
		}

		size_t size = 0;

		for (auto& [key, value] : component_classes_) {
			value->add_component();
			size = value->size();
		}

		return size;
	}

	void remove_entity(size_t id) {
		deleted_components_.push_back(id);

		for (auto& [key, value] : component_classes_) {
			value->delete_component(id);
		}
	}



};

template<typename T> 
T* ComponentManager::get_component(size_t e) {
	assert(component_classes_.size());
	assert(component_classes_.contains(typeid(T).hash_code()));
	assert(e != 0);
	auto& component_list = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
	assert(e < component_list.size());
	auto& component_opt = component_list.components_.at(e);

	if (!component_opt.has_value()) return nullptr;
	return &component_opt.get()
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

struct ComponentLife {};

int main(int, char**) {
	ComponentManager component_manager;

	component_manager.add_component_class<ComponentLife>();

	auto player = component_manager.add_entity();
	auto player_physics = component_manager.get_component<Physics>(player);

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