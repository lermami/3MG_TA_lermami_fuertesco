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

class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  void WaitResources();

  void LoadObj(Engine& e, const char* name, const char* path);
  Geometry* getGeometry(std::string nameID);

  void loadTexture(const char* name, Texture tex, const char* path);
  unsigned getTexture(const char* name);

  bool createVertexBuffer(std::string nameID, float* vertices, unsigned size);
  VertexBuffer* getVertexBuffer(std::string nameID);

  bool createIndexBuffer(std::string nameID, unsigned* indices, unsigned size);
  IndexBuffer* getIndexBuffer(std::string nameID);

  bool createBuffersWithGeometry(Geometry* geo, std::string nameIDVertex, std::string nameIDIndex);
private:
  Geometry LoadObj(const char* name, const char* path);

  std::unordered_map<std::string, unsigned> textures_;

  std::unordered_map<std::string, std::future<Geometry>> geometryFutures_;
  std::unordered_map<std::string, Geometry> geometries_;

  std::unordered_map<std::string, VertexBuffer*> vertexBuffers_;
  std::unordered_map<std::string, IndexBuffer*> indexBuffers_;

};