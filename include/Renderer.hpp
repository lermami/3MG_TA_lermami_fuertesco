/**
 * @file Renderer.hpp
 * @brief Header file for the Renderer class.
 *
 * @defgroup Renderer Renderer class
 * @brief This file defines the Renderer class, which renders all entities on the engine.
 */
#pragma once

#include "glm/glm.hpp"
#include "texture.hpp"

//TODO: delete this
#include "shader_management.hpp"

class Engine;
class Window;
class Vec3;

/**
 * @brief Manages the rendering pipeline for the engine, including lights, shadows, and scene geometry.
 */
class Renderer {
public:

  /**
     * @brief Constructor for Renderer class.
     * Takes references to the Engine and Window objects for interaction.
     *
     * @param e Reference to the Engine object.
     * @param w Reference to the Window object.
     */
  Renderer(Engine& e, Window& w);

  /**
     * @brief Destructor for Renderer class.
     * Performs any necessary cleanup for the rendering resources.
     */
  ~Renderer();

  /**
     * @brief Ads to rendering all lights in the scene.
     * This function sets up shader light uniforms.
     */
  void renderLights();

  /**
     * @brief Calculates the shadow matrices for all lights with shadows enabled.
     * This function involves using the light position and direction to calculate the projection and view matrices for shadow mapping.
     */
  void CalculateShadowsMatrix();

  /**
     * @brief Renders the entire scene.
     * This function involves setting up shader uniforms, drawing scene geometry, and potentially post-processing effects.
     */
  void render();

  /**
     * @brief Renders the shadow map for a specific shader program.
     *
     * @param program The shader program to use for rendering the shadow map.
     */
  void renderShadowMap(unsigned int program);

  /**
     * @brief Configures the shadow matrix based on near and far planes, light position, and direction.
     *
     * @param near The near plane distance for the shadow frustum.
     * @param far The far plane distance for the shadow frustum.
     * @param pos The position of the light source.
     * @param direction The direction of the light.
     * @return The calculated shadow matrix.
     */
  glm::mat4 ConfigureShadowMatrix(float near, float far, Vec3 pos, Vec3 direction);

private:

  /**
     * @brief Renders ImGui elements for debugging or visualization purposes.
     */
  void renderImgui();

  Engine& engine_; /**< Reference to the Engine object. */
  Window& window_; /**< Reference to the Window object. */

  bool renderShadows_; /**< Flag indicating whether to render shadows. */
  unsigned int shadow_resolution_; /**< Resolution of the shadow map texture in both direction W & H. */
  Texture depthmap_; /**< Handle to the depth map texture. */
  unsigned depthmapFBO_; /**< Handle to the framebuffer object used for shadow mapping. */
  Shader shadowProgram_; /**< Handle to the shader program used for shadow rendering. */
};