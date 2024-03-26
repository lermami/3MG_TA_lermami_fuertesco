/**
 * @file Engine.hpp
 * @brief Header file for the Engine class.
 *
 * @defgroup Engine Engine class
 * @brief This file defines the Engine class, which manage all engine main process.
 */
#pragma once
#include<memory>

struct ComponentManager;
class ResourceManager;
class ThreadManager;
class CameraManager;

struct GlewResource {
  GlewResource();
  ~GlewResource();
};

/**
 * @brief Main class representing the game engine, managing core subsystems.
 */
class Engine {
public:

  /**
   * @brief Constructs the Engine object, initializing all subsystems.
   */
  Engine();

  /**
   * @brief Destructs the Engine object, cleaning up resources.
   */
  ~Engine();

  /**
   * @brief Retrieves a reference to the ComponentManager for accessing components.
   * @return Reference to the ComponentManager.
   */
  ComponentManager& getComponentManager();

  /**
   * @brief Retrieves a reference to the ResourceManager for managing resources.
   * @return Reference to the ResourceManager.
   */
  ResourceManager& getResourceManager();

  /**
   * @brief Retrieves a reference to the ThreadManager for managing threads.
   * @return Reference to the ThreadManager.
   */
  ThreadManager& getThreadManager();
  CameraManager& getCameraManager();

private:

  /**
   * @brief Manages game components and their interactions.
   */
  std::unique_ptr<ComponentManager> componentM_;

  /**
   * @brief Manages game resources, such as textures and models.
   */
  std::unique_ptr<ResourceManager> resourceM_;

  /**
   * @brief Manages threading for multithreaded tasks.
   */
  std::unique_ptr<ThreadManager> threadM_;
  std::unique_ptr<CameraManager> cameraM_;

  GlewResource glew_resource_;
};