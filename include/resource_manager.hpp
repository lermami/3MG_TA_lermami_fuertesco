#pragma once

#include "Engine.hpp"
#include "Window.hpp"
#include <vector>
#include "texture.hpp"


class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  unsigned loadTexture(const char* name, Texture tex, const char* path);

  unsigned getTexture(const char* name);
  unsigned getTexture(int index);
  std::string getTextureName(unsigned value);

  std::vector<unsigned>& getTextureList();
  std::vector<std::string>& getTextureNamesList();

private:
  std::vector<unsigned> textures_;
  std::vector<std::string> texture_names_;

};