#pragma once

#include "Engine.hpp"
#include "Window.hpp"
#include <vector>
#include "texture.hpp"
#include <unordered_map>

struct Geometry;
class VertexBuffer;

class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  unsigned loadTexture(const char* name, Texture tex, const char* path);
  Geometry LoadObj(const char* name, const char* path);

  unsigned getTexture(const char* name);
  unsigned getTexture(int index);
  std::string getTextureName(unsigned value);

  std::vector<unsigned>& getTextureList();
  std::vector<std::string>& getTextureNamesList();

  bool createVertexBuffer(std::string nameID, float* vertices, unsigned size);
  VertexBuffer* getVertexBuffer(std::string nameID);

private:
  std::vector<unsigned> textures_;
  std::vector<std::string> texture_names_;

  std::vector<Geometry> geometries_;
  std::vector<std::string> geometry_names_;

  std::unordered_map<std::string, VertexBuffer*> vertexBuffers_;

};