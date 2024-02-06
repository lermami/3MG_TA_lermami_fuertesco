#pragma once
#include<vector>
#include<memory>

#include "enum.hpp"

struct ComponentManager;
struct Geometry;

class Engine {
public:
  Engine();
  ~Engine();

  static Geometry LoadObj(const char* path);

  ComponentManager& getComponentManager();

private:
  std::unique_ptr<ComponentManager> componentM_;
};