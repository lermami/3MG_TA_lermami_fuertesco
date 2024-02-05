#pragma once
#include <vector>


class ComponentManager;

class Engine {
  friend class ComponentManager;
public:
  Engine();
  ~Engine();

  static Geometry LoadObj(const char* path);

  ComponentManager* getComponentManager();

private:
  ComponentManager* componentM_;

};