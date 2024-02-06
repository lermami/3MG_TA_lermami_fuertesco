#include "component_manager.hpp"

ComponentManager::ComponentManager(){
		add_component_class<RenderComponent>();
		add_component_class<TransformComponent>();
		add_component_class<AudioComponent>();
		add_component_class<LightComponent>();
		add_component_class<CameraComponent>();
}

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