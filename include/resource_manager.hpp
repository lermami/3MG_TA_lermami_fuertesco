/**
 * @file ResourceManager.hpp
 * @brief Header file for the ResourceManager class.
 *
 * @defgroup ResourceManager Resource Manager
 * @brief This file defines the ResourceManager class, which handles loading and managing resources like textures, geometries, and buffers.
 */
#pragma once

#include "Engine.hpp"
#include "Window.hpp"
#include <vector>
#include "texture.hpp"
#include <unordered_map>
#include <future>

struct Geometry;
class VertexBuffer;
class IndexBuffer;

/**
 * @class ResourceManager
 * @brief This class manages the loading and access of various resources for the engine.
 *
 * The ResourceManager class provides a central system for loading textures, geometries,
 *  vertex buffers, and index buffers. It allows asynchronous loading, waiting for resources
 *  to be ready, and retrieval of resources by name.
 *
 * @ingroup ResourceManager
 */
class ResourceManager {
public:
  /**
   * Constructor for the ResourceManager class.
   */
  ResourceManager();

  /**
   * Destructor for the ResourceManager class.
   *
   * Releases resources and cleans up internal data.
   */
  ~ResourceManager();

  /**
   * Blocks execution until all pending resource loading tasks are finished.
   */
  void WaitResources();

  /**
   * Loads an OBJ geometry file asynchronously.
   *
   * This function queues a loading task for an OBJ file, associating it with the provided name.
   *
   * @param e Reference to the Engine object.
   * @param name The name to associate with the loaded geometry.
   * @param path The path to the OBJ file to load.
   */
  void LoadObj(Engine& e, const char* name, const char* path);

  /**
   * Retrieves a previously loaded geometry by name.
   *
   * @param nameID The name associated with the geometry to retrieve.
   * @return A pointer to the geometry object, or nullptr if not found.
   */
  Geometry* getGeometry(std::string nameID);

  /**
   * Loads a texture from a file.
   *
   * @param name The name to associate with the loaded texture.
   * @param tex An existing Texture object to load the texture data into.
   * @param path The path to the texture file to load.
   */
  void loadTexture(const char* name, Texture tex, const char* path);

  /**
   * Retrieves a previously loaded texture by name.
   *
   * @param name The name associated with the texture to retrieve.
   * @return The ID of the texture, or 0 if not found.
   */
  unsigned getTexture(const char* name);

  /**
   * Creates a vertex buffer with the given data.
   *
   * @param nameID The name to associate with the vertex buffer.
   * @param vertices A pointer to the vertex data.
   * @param size The size of the vertex data in bytes.
   * @return True if the vertex buffer was created successfully, false otherwise.
   */
  bool createVertexBuffer(std::string nameID, float* vertices, unsigned size);

  /**
   * Retrieves a previously created vertex buffer by name.
   *
   * @param nameID The name associated with the vertex buffer to retrieve.
   * @return A pointer to the vertex buffer object, or nullptr if not found.
   */
  VertexBuffer* getVertexBuffer(std::string nameID);

  /**
   * Creates an index buffer with the given data.
   *
   * @param nameID The name to associate with the index buffer.
   * @param indices A pointer to the index data.
   * @param size The size of the index data in bytes.
   * @return True if the index buffer was created successfully, false otherwise.
   */
  bool createIndexBuffer(std::string nameID, unsigned* indices, unsigned size);

  /**
   * Retrieves a previously created index buffer by name.
   *
   * @param nameID The name associated with the index buffer to retrieve.
   * @return A pointer to the index buffer object, or nullptr if not found.
   */
  IndexBuffer* getIndexBuffer(std::string nameID);

  /**
  * Creates vertex and index buffers from a loaded Geometry object.
  *
  * This function creates vertex and index buffers from the data stored in a Geometry object.
  *  It assigns the provided names to the created buffers.
  *
  * @param geo A pointer to the Geometry object containing the data.
  * @param nameIDVertex The name to associate with the vertex buffer.
  * @param nameIDIndex The name to associate with the index buffer.
  * @return True if both buffers were created successfully, false otherwise.
  */
  bool createBuffersWithGeometry(Geometry* geo, std::string nameIDVertex, std::string nameIDIndex);
private:
  /**
   * Internal implementation for loading an OBJ file.
   *
   * This function is private as it's not intended for external use.
   *  It performs the actual loading of the OBJ file data.
   *
   * @param name The name to associate with the loaded geometry.
   * @param path The path to the OBJ file to load.
   * @return A Geometry object containing the loaded data, or nullptr if loading failed.
   */
  Geometry LoadObj(const char* name, const char* path);

  std::unordered_map<std::string, unsigned> textures_;

  std::unordered_map<std::string, std::future<Geometry>> geometryFutures_;
  std::unordered_map<std::string, Geometry> geometries_;

  std::unordered_map<std::string, VertexBuffer*> vertexBuffers_;
  std::unordered_map<std::string, IndexBuffer*> indexBuffers_;

};