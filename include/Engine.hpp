#pragma once

class Engine {

public:
  static void init();
  static void terminate();

  ~Engine() = delete;
private:
  Engine() = default;
};