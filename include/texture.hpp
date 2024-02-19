#pragma once

#include "enum.hpp"

class Texture {
public:
  Texture(TextureType type, TextureFormat format);
  ~Texture();

  unsigned LoadTexture(const char* path);

  void set_wrap_s(TextureWrap param);
  void set_wrap_t(TextureWrap param);
  void set_min_filter(TextureFiltering param);
  void set_mag_filter(TextureFiltering param);

private:
  unsigned handle_;
  TextureType type_;
  TextureFormat format_;
  TextureWrap wrap_s_format_;
  TextureWrap wrap_t_format_;
  TextureFiltering min_filter_format_;
  TextureFiltering mag_filter_format_;
};