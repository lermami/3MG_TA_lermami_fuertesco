#include "component_manager.hpp"

size_t ComponentManager::add_entity() {
	if (deleted_components_.size() > 0) {
		size_t id = deleted_components_.back();
		deleted_components_.pop_back();

		for (auto& [key, value] : component_classes_) {
			value->add_component((int)id);
		}
		return id;
	}

	size_t size = 0;

	for (auto& [key, value] : component_classes_) {
		value->add_component();
		size = value->size();
	}

	return size;
}

void ComponentManager::remove_entity(size_t id) {
	deleted_components_.push_back(id);

	for (auto& [key, value] : component_classes_) {
		value->delete_component(id);
	}
}