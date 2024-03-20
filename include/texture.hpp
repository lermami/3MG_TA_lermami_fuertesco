/**
 * @file Texture.hpp
 * @brief Header file for the Texture class.
 *
 * @defgroup Texture Texture class
 * @brief This file defines the Texture class, which represents a texture object used for rendering.
 */
#pragma once

#include "enum.hpp"
#include <optional>

struct TextureInfo {
  TextureInfo(TextureTarget target, TextureFormat format, TextureType type,
    TextureWrap wrap_s = TextureWrap::kRepeat, TextureWrap wrap_t = TextureWrap::kRepeat,
    TextureFiltering min_filter = TextureFiltering::kLinear, TextureFiltering mag_filter = TextureFiltering::kLinear);

  TextureType type_;
  TextureTarget target_;
  TextureFormat format_;
  TextureWrap wrap_s_format_;
  TextureWrap wrap_t_format_;
  TextureFiltering min_filter_format_;
  TextureFiltering mag_filter_format_;
};

/**
* @class Texture
* @brief This class represents a texture object used for rendering.
*
* The Texture class provides functionality to load and manage textures for use in
*  rendering pipelines. It allows loading textures from files or creating them in memory,
*  and setting various texture parameters like wrapping mode and filtering.
*
* @ingroup Texture
*/
class Texture {
public:
  /**
  * Destructor for the Texture class.
  *
  * Releases resources associated with the texture object.
  */
  ~Texture();

  /**
  * Loads a texture from a file path.
  *
  * Loads image data from the specified file path and creates a texture object.
  *
  * @param path The path to the image file.
  * @return The ID of the loaded texture, or 0 if loading failed.
  */
  static Texture LoadTexture(TextureInfo info, const char* path);

  /**
   * Loads a texture in memory.
   *
   * Creates a texture object with the specified width, height, and format.
   *  The caller is responsible for providing the pixel data.
   *
   * @param w Width of the texture in pixels.
   * @param h Height of the texture in pixels.
   * @return The ID of the loaded texture, or 0 if creation failed.
   */
  static Texture LoadTexture(TextureInfo info, int w, int h);

  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  Texture(Texture&& o);
  Texture& operator=(Texture&& o);

  unsigned get();

private:
  /**
  * Constructor for the Texture class.
  *
  * Initializes a Texture object with the specified target, format, and type.
  *
  * @param target The target of the texture.
  * @param format The format of the texture data.
  * @param type The type of the texture data.
  */
  Texture(unsigned handle);

  unsigned handle_;
  bool destroy_;
};

