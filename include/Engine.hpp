#pragma once
#include <vector>
#include "component_manager.hpp"


class Engine {
public:
  Engine();
  ~Engine();

  static Geometry LoadObj(const char* path);

};