#pragma once
#include<vector>
#include<memory>

#include "enum.hpp"
#include "resource_manager.hpp"

struct ComponentManager;
class ResourceManager;


class Engine {
public:
  Engine();
  ~Engine();

  ComponentManager& getComponentManager();
  ResourceManager& getResourceManager();

private:
  std::unique_ptr<ComponentManager> componentM_;
  std::unique_ptr<ResourceManager> resourceM_;
};