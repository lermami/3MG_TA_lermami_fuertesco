#pragma once
#include<unordered_map>
#include<memory>
#include<optional>
#include<cassert>
#include<utility>

#include "default_components.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ComponentManager {
	std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_classes_;
	std::vector <size_t> deleted_components_;

	ComponentManager();
	~ComponentManager() = default;

	template<typename T> void add_component_class();
	template<typename T> void create_component(int position, T& component);
	template<typename T> T* get_component(size_t e);
	template<typename T> std::vector < std::optional<T>>* get_component_list();

	template<class ... T> size_t add_entity(T... components);
	void remove_entity(size_t id);

};

template<typename T> void ComponentManager::add_component_class() {
	component_classes_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
}

template<typename T> T* ComponentManager::get_component(size_t e) {
	assert(component_classes_.size());
	assert(e != 0);
	//Entities start on 1
	e = e - 1;
	auto& component_l = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
	assert(e < component_l.size());
	auto& component_opt = component_l.components_.at(e);

	if (!component_opt.has_value()) return nullptr;
	return &component_opt.value();
}

template<typename T> void ComponentManager::create_component(int position, T& component) {
	//Cast to the specific component list and add the components
	component_list<T>* list = dynamic_cast<component_list<T>*>(component_classes_[typeid(T).hash_code()].get());
	list->add_component_at(position, component);
}

template<typename T> std::vector < std::optional<T>>* ComponentManager::get_component_list(){
	assert(component_classes_.size());
	auto& component_l = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
	auto& component_opt = component_l.components_;

	return &component_opt;
}

template<class ... T> size_t ComponentManager::add_entity(T... components){
	if (deleted_components_.size() > 0) {
		size_t id = deleted_components_.back();
		deleted_components_.pop_back();

		for (auto& [key, value] : component_classes_) {
			value->add_component((int)id);
		}
		return id;
	}

	size_t size = 0;

	//Add nullopt to components that not matches with the parameters
	for (auto& [key, value] : component_classes_) {
		value->add_component();
		size = value->size();
	}

	//Fold expression to iterate in parameters pack (components)
	(create_component(size, components), ...);

	//Return entity value
	return size;
}