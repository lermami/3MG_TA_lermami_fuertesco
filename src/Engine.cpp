#include "Engine.hpp"

#include "component_manager.hpp"
#include "camera.hpp"
#include "thread_manager.hpp"
#include "resource_manager.hpp"

#include "GLFW/glfw3.h"

#include <time.h>

Engine::Engine() : componentM_{ std::make_unique<ComponentManager>() }, resourceM_{ std::make_unique<ResourceManager>() }, 
		threadM_{ std::make_unique<ThreadManager>() }, cameraM_{ std::make_unique<CameraManager>(*this) } {

	srand((unsigned int)time(NULL));
}

Engine::~Engine(){
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

CameraManager& Engine::getCameraManager() {
	return *cameraM_;
}

GlewResource::GlewResource()
{
	glfwInit();
}

GlewResource::~GlewResource()
{
	glfwTerminate();
}
