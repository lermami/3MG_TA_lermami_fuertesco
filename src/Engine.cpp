#include "Engine.hpp"
#include "component_manager.hpp"
#include "default_components.hpp"
#include "thread_manager.hpp"

#include "GLFW/glfw3.h"


#include <time.h>

Engine::Engine() : componentM_{ std::make_unique<ComponentManager>() }, resourceM_{ std::make_unique<ResourceManager>() }, threadM_{ std::make_unique<ThreadManager>() } {
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

ThreadManager& Engine::getThreadManager() {
	return *threadM_;
}
