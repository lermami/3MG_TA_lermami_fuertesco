#pragma once
#include<vector>
#include<memory>

#include "enum.hpp"
#include "resource_manager.hpp"

struct ComponentManager;
class ResourceManager;
class ThreadManager;
class CameraManager;


class Engine {
public:
  Engine();
  ~Engine();

  ComponentManager& getComponentManager();
  ResourceManager& getResourceManager();
  ThreadManager& getThreadManager();
  CameraManager& getCameraManager();

private:
  std::unique_ptr<ComponentManager> componentM_;
  std::unique_ptr<ResourceManager> resourceM_;
  std::unique_ptr<ThreadManager> threadM_;
  std::unique_ptr<CameraManager> cameraM_;
};