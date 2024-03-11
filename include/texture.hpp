/**
 * @file Texture.hpp
 * @brief Header file for the Texture class.
 *
 * @defgroup Texture Texture class
 * @brief This file defines the Texture class, which represents a texture object used for rendering.
 */
#pragma once

#include "enum.hpp"

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
   * Constructor for the Texture class.
   *
   * Initializes a Texture object with the specified target, format, and type.
   *
   * @param target The target of the texture.
   * @param format The format of the texture data.
   * @param type The type of the texture data.
   */
  Texture(TextureTarget target, TextureFormat format, TextureType type);

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
  unsigned LoadTexture(const char* path);

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
  unsigned LoadTexture(int w, int h);

  /**
   * Sets the wrap mode for the S coordinate of the texture.
   *
   * The wrap mode determines how texture coordinates outside the [0, 1] range are handled.
   *
   * @param param The wrap mode to use.
   */
  void set_wrap_s(TextureWrap param);

  /**
   * Sets the wrap mode for the T coordinate of the texture.
   *
   * The wrap mode determines how texture coordinates outside the [0, 1] range are handled.
   *
   * @param param The wrap mode to use.
   */
  void set_wrap_t(TextureWrap param);

  /**
  * Sets the minification filter for the texture.
  *
  * The minification filter determines how the texture is sampled when it's being shrunk.
  *
  * @param param The minification filter to use.
  */
  void set_min_filter(TextureFiltering param);

  /**
  * Sets the magnification filter for the texture.
  *
  * The magnification filter determines how the texture is sampled when it's being magnified.
  *
  * @param param The magnification filter to use.
  */
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