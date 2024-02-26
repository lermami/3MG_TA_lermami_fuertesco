#pragma once
#include<vector>
#include<memory>

#include "enum.hpp"
#include "resource_manager.hpp"

struct ComponentManager;
struct ResourceManager;
struct Geometry;


class Engine {
public:
  Engine();
  ~Engine();

  static Geometry LoadObj(const char* path);

  ComponentManager& getComponentManager();
  ResourceManager& getResourceManager();

private:
  std::unique_ptr<ComponentManager> componentM_;
  std::unique_ptr<ResourceManager> resourceM_;
};