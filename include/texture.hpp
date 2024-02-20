#pragma once

#include "enum.hpp"

class Texture {
public:
  Texture(TextureTarget target, TextureFormat format, TextureType type);
  ~Texture();

  //With path
  unsigned LoadTexture(const char* path);

  //Without path
  unsigned LoadTexture(int w, int h);

  void set_wrap_s(TextureWrap param);
  void set_wrap_t(TextureWrap param);
  void set_min_filter(TextureFiltering param);
  void set_mag_filter(TextureFiltering param);

private:
  unsigned handle_;
  TextureType type_;
  TextureTarget target_;
  TextureFormat format_;
  TextureWrap wrap_s_format_;
  TextureWrap wrap_t_format_;
  TextureFiltering min_filter_format_;
  TextureFiltering mag_filter_format_;
};