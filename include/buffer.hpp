/**
 * @file buffer.hpp
 * @brief Header file for the VertexBuffer and IndexBuffer classes.
 *
 * @defgroup Buffers Vertex and Index Buffers
 * @brief This file defines classes for managing vertex and index buffers in OpenGL.
 */

#pragma once
#include <memory>
#include "enum.hpp"

 /**
  * @class VertexBuffer
  * @brief This class represents a vertex buffer object (VBO) in OpenGL.
  *
  * The VertexBuffer class encapsulates the creation, binding, and data upload for a
  *  vertex buffer object (VBO) in OpenGL. It's primarily used by the ResourceManager.
  *
  * @ingroup Buffers
  */
class VertexBuffer {
  friend class ResourceManager;
public:
  /**
   * Destructor for the VertexBuffer class.
   *
   * Releases the underlying OpenGL vertex buffer object.
   */
  ~VertexBuffer();

  /**
   * Binds the vertex buffer object for use in rendering or update values.
   */
  void bind() const;

  /**
  * Unbinds the vertex buffer object, disabling its use in rendering.
  */
  void unbind() const;

  /**
   * Retrieves the handle of the underlying OpenGL vertex buffer object.
   *
   * @return The unsigned integer handle representing the VBO.
   */
  unsigned get() const;

  /**
   * Retrieves the number of vertices stored in the vertex buffer.
   *
   * @return The count of vertices in the VBO.
   */
  unsigned getCount() const;

  /**
   * Uploads float attribute data to the vertex buffer.
   *
   * This method uploads a specific attribute to the vertex buffer, such as
   *  position, color, or texture coordinates.
   *
   * @param id The location of the attribute in the vertex shader.
   * @param size The number of components for the attribute.
   * @param stride The byte offset between consecutive vertices in the buffer.
   * @param offset The byte offset within each vertex for the specific attribute.
   */
  void uploadFloatAttribute(unsigned int id, int size, int stride, void* offset);

private:
  /**
   * Private constructor to create a vertex buffer from raw vertex data.
   *
   * @param vertices A pointer to the array of floating-point vertex data.
   * @param size The number of vertices in the array.
   */
  VertexBuffer(float* vertices, unsigned size);
  unsigned handle_;
  unsigned count_;
};

/**
 * @class IndexBuffer
 * @brief This class represents an index buffer object (IBO) in OpenGL.
 *
 * The IndexBuffer class encapsulates the creation, binding, and data usage of an
 *  index buffer object (IBO) in OpenGL. It's primarily used by the ResourceManager.
 *
 * @ingroup Buffers
 */
class IndexBuffer {
  friend class ResourceManager;
public:
  /**
   * Destructor for the IndexBuffer class.
   *
   * Releases the underlying OpenGL index buffer object.
   */
  ~IndexBuffer();

  /**
  * Binds the index buffer object for use in rendering.
  */
  void bind() const;

  /**
   * Unbinds the index buffer object, disabling its use in rendering.
   */
  void unbind() const;

  /**
   * Retrieves the handle of the underlying OpenGL index buffer object.
   *
   * @return The unsigned integer handle representing the IBO.
   */
  unsigned get() const;

  /**
  * Retrieves the number of indices stored in the index buffer.
  *
  * @return The count of indices in the IBO.
  */
  unsigned getCount() const;

private:
  /**
   * Private constructor to create an index buffer from raw index data.
   *
   * @param vertices A pointer to the array of unsigned integer index data.
   * @param size The number of indices in the array.
   */
  IndexBuffer(unsigned* vertices, unsigned size);
  unsigned handle_;
  unsigned count_;
};