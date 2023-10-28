#include<unordered_map>
#include<memory>
#include<optional>
#include<cassert>

struct Position {
	int x, y;
};

struct Physics {
	float acceleration;
};

struct Material {
};

struct component_base {
	virtual void add_component() = 0;
	virtual void add_component(int position) = 0;
	virtual void delete_component(size_t id) = 0;
	virtual size_t size() = 0;
};

template<typename T>
struct component_list : component_base {
	std::vector < std::optional<T>> components_;

	virtual void add_component() override {
		T a;
		components_.emplace_back(a);
	}

	virtual void add_component(int position) override {
		T a;
		//Minus 1 because position refers to an entity that starts in 1
		components_[position-1] = a;
	}

	virtual size_t size() override {
		return components_.size();
	}

	virtual void delete_component(size_t id) override {
		assert(id != 0);
		components_[id-1].reset();
		components_[id-1] = std::nullopt;
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

	template<typename T>
	T* get_component(size_t e) {
		assert(component_classes_.size());
		//assert(component_classes_.contains(typeid(T).hash_code()));
		assert(e != 0);
		//Entities start on 1
		e = e - 1;
		auto& component_l = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
		assert(e < component_l.size());
		auto& component_opt = component_l.components_.at(e);

		if (!component_opt.has_value()) return nullptr;
		return &component_opt.value();
	}

	size_t add_entity();

	void remove_entity(size_t id);

};