#include "resource_manager.hpp"
#include <tuple>


ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
  //TODO: handle 
}

unsigned ResourceManager::loadTexture(const char* name, Texture tex, const char* path) {
  unsigned new_tex = tex.LoadTexture(path);

  textures_.push_back(new_tex);
  texture_names_.push_back(name);

  return new_tex;
}

unsigned ResourceManager::getTexture(const char* name) {
  int it = 0;
  for (auto& tex_name : texture_names_) {
    if (tex_name == name) {
      return textures_[it];
    }
    it++;
  }

  return 0;
}

unsigned ResourceManager::getTexture(int index) {
  if (index > textures_.size() || index < 0) return 0;

  return textures_[index];
}

std::string ResourceManager::getTextureName(unsigned value) {
  int it = 0;

  for (auto& tex_value : textures_) {
    if (tex_value == value) {
      return texture_names_[it];
    }

    it++;
  }
}

std::vector<unsigned>& ResourceManager::getTextureList(){
  return textures_;
}

std::vector<std::string>& ResourceManager::getTextureNamesList() {
  return texture_names_;
}