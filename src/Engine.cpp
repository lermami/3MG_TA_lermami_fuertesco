#include "Engine.hpp"
#include "component_manager.hpp"
#include "default_components.hpp"

#include "GLFW/glfw3.h"


#include <time.h>

Engine::Engine() : componentM_{ std::make_unique<ComponentManager>() }, resourceM_{std::make_unique<ResourceManager>()} {
  glfwInit();

	srand((unsigned int)time(NULL));
}

Engine::~Engine(){
  glfwTerminate();
}

ComponentManager& Engine::getComponentManager() {
	return *componentM_;
}

ResourceManager& Engine::getResourceManager() {
	return *resourceM_;
}

