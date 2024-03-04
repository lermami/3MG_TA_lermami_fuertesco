#include "component_manager.hpp"

ComponentManager::ComponentManager(){
		add_component_class<RenderComponent>();
		add_component_class<TransformComponent>();
		add_component_class<AudioComponent>();
		add_component_class<LightComponent>();
		add_component_class<CameraComponent>();
		add_component_class<ColorComponent>();
		add_component_class<BoxColliderComponent>();
}

void ComponentManager::remove_entity(size_t id) {
	deleted_components_.push_back(id);

	for (auto& [key, value] : component_classes_) {
		value->delete_component(id);
	}
}

