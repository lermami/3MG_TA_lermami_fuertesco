#pragma once

enum class TextureType {
  kTexture_1D = 0x0DE0,
  kTexture_2D = 0x0DE1,
  kTexture_3D = 0x806F,
};

enum class TextureFormat {
  kRGB = 0x1907,
  kRGBA = 0x1908,
};

enum class TextureParameters {
  kRepeat,
  kLinear,
};


class Texture {
public:
  Texture(TextureType type, TextureFormat format);
  ~Texture();

  unsigned LoadTexture(const char* path);

  void set_wrap_s(TextureParameters param);
  void set_wrap_t(TextureParameters param);
  void set_min_filter(TextureParameters param);
  void set_mag_filter(TextureParameters param);

private:
  unsigned handle_;
  TextureType type_;
  TextureFormat format_;
  TextureParameters wrap_s_format_;
  TextureParameters wrap_t_format_;
  TextureParameters min_filter_format_;
  TextureParameters mag_filter_format_;
};