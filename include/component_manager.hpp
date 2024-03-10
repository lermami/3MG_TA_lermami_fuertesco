/**
 * @file ComponentManager.hpp
 * @brief Header file for the ComponentManager class.
 *
 * @defgroup EntityManager Entity Manager
 * @brief This file defines the ComponentManager class, which manages the components attached to entities in the game world.
 */
#pragma once
#include<unordered_map>
#include<memory>
#include<optional>
#include<cassert>
#include<utility>

#include "default_components.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

 /**
	* @class ComponentManager
	* @brief This class manages the components attached to entities in the game world.
	*
	* The ComponentManager class provides functionality to register component types,
	*  create and access components for entities, and add/remove entities. It uses
	*  an entity-component-system (ECS) approach for managing game objects.
	*
	* @ingroup EntityManager
	*/
struct ComponentManager {
	std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_classes_;
	std::vector <size_t> deleted_components_;

	/**
	* Constructor for the ComponentManager class.
	*
	* Adds default components.
	*/
	ComponentManager();

	/**
	 * Destructor for the ComponentManager class.
	 *
	 */
	~ComponentManager() = default;

	/**
	 * Template method to register a component class with the manager.
	 *
	 * This template method allows registering a specific component type with the manager.
	 *  It internally creates a component list to store instances of that type.
	 *
	 * @tparam T The type of the component to register.
	 */
	template<typename T> void add_component_class();

	/**
	* Template method to create a component of a specific type for an entity.
	*
	* This template method creates a component of the specified type and associates it with an entity.
	*  It takes the entity ID and a reference to the component data as arguments.
	*
	* @tparam T The type of the component to create.
	* @param position Index at which to insert the component in the entity's component list (optional).
	* @param component Reference to the component data to create.
	*/
	template<typename T> void create_component(int position, T& component);

	/**
	 * Template method to retrieve a component of a specific type for an entity.
	 *
	 * This template method retrieves a component of the specified type for an entity identified by ID.
	 *  It returns a pointer to the component data if it exists, or nullptr otherwise.
	 *
	 * @tparam T The type of the component to retrieve.
	 * @param e The ID of the entity to retrieve the component from.
	 * @return A pointer to the component data, or nullptr if not found.
	 */
	template<typename T> T* get_component(size_t e);

	/**
	 * Template method to retrieve a list of optional components of a specific type for all entities.
	 *
	 * This template method retrieves a reference to a vector containing std::optional objects for
	 *  components of the specified type. Each element in the vector corresponds to an entity,
	 *  and the optional object holds the component data if it exists for that entity.
	 *
	 * @tparam T The type of the component to retrieve.
	 * @return A reference to the vector containing std::optional objects for the components.
	 */
	template<typename T> std::vector < std::optional<T>>* get_component_list();

	/**
	* Template method to create a new entity with optional components.
	*
	* This template method creates a new entity and optionally adds components of the specified types.
  * It takes a variadic template parameter pack to represent the component types.
	* 
	* @tparam ... Ts Additional component types to add to the entity (variadic template parameter pack).
	* @return The ID of the newly created entity.
	*/
	template<class ... T> size_t add_entity(T... components);

	/**
	 * Method to remove an entity from the manager.
	 *
	 * This method removes an entity identified by ID from the manager. It marks the entity's
	 *  components for potential reuse and updates internal data structures.
	 *
	 * @param id The ID of the entity to remove.
	 */
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