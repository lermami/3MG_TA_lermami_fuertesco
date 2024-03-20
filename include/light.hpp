/**
 * @file light.hpp
 * @brief Header file for the LightComponent class.
 *
 * @defgroup LightComponent LightComponent class
 * @brief This file defines the LightComponent class, which adds information to render lights.
 */
#pragma once

#include "vector_3.hpp"
#include "enum.hpp"

#include "glm/glm.hpp"

struct DotShadowMatrix {
  glm::mat4 m[6];
};

struct LightComponent{
public:

  /**
 * @brief Default constructor for LightComponent.
 * Sets all member variables to default values.
 */
  LightComponent();

  /**
 * @brief Constructor for LightComponent to init an Ambient Light.
 *
 * Initializes the light with the provided color and sets other member variables to defaults.
 *
 * @param color The color of the light.
 */
  LightComponent(Vec3 color);

  /**
 * @brief Constructor for LightComponent to init a Directional Light.
 *
 * Initializes the light as a directional light with the provided direction, color, specular color,
 * minimum and maximum shadow rendering distances.
 *
 * @param direction The direction of the light.
 * @param color The color of the light.
 * @param specular The specular color of the light.
 * @param min_shadow_render_distance The minimum distance for shadow rendering.
 * @param max_shadow_render_distance The maximum distance for shadow rendering.
 */
  LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);

  /**
 * @brief Constructor for LightComponent to init a point light.
 *
 * Initializes the light as a point light with the provided color, specular color, constant, linear,
 * and quadratic attenuation factors, minimum and maximum shadow rendering distances.
 *
 * @param color The color of the light.
 * @param specular The specular color of the light.
 * @param constant The constant attenuation factor.
 * @param linear The linear attenuation factor.
 * @param quadratic The quadratic attenuation factor.
 * @param min_shadow_render_distance The minimum distance for shadow rendering.
 * @param max_shadow_render_distance The maximum distance for shadow rendering.
 */
  LightComponent(Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);

  /**
 * @brief Constructor for LightComponent to init a spot light.
 *
 * Initializes the light as a spot light with the provided direction, color, specular color, constant,
 * linear, and quadratic attenuation factors, cutoff angle, minimum and maximum shadow rendering distances.
 *
 * @param direction The direction of the light.
 * @param color The color of the light.
 * @param specular The specular color of the light.
 * @param constant The constant attenuation factor.
 * @param linear The linear attenuation factor.
 * @param quadratic The quadratic attenuation factor.
 * @param cutoff_angle The cutoff angle of the spot light (in radians).
 * @param min_shadow_render_distance The minimum distance for shadow rendering.
 * @param max_shadow_render_distance The maximum distance for shadow rendering.
 */
  LightComponent(Vec3 direction, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle, float min_shadow_render_distance = 0.01f, float max_shadow_render_distance = 1000);

  /**
 * @brief Destructor for LightComponent.
 * 
 */
  ~LightComponent();

  LightType type_;/**< @brief The type of the light (ambient, directional, point, or spot).*/

  Vec3 color_;/**< @brief The color of the light.*/
  Vec3 spec_color_;/**< @brief The specular color of the light (used for specular lighting calculations).*/

  Vec3 direction_;/**< @brief The direction of the light (only relevant for directional and spot lights).*/

  float constant_;/**< @brief The constant attenuation factor of the light (used for distance-based attenuation) (Not used for ambient lights)*/
  float linear_;/**< @brief The linear attenuation factor of the light (used for distance-based attenuation) (Not used for ambient lights)*/
  float quadratic_;/**< @brief The quadratic attenuation factor of the light (used for distance-based attenuation) (Not used for ambient lights)*/

  float cutoff_angle_;/**< @brief The cutoff angle of the spot light (in radians) (only relevant for spot lights).*/

  float min_shadow_render_distance_;/**< @brief The minimum distance at which shadows are rendered for this light. (Not used for ambient lights)*/
  float max_shadow_render_distance_;/**< @brief The maximum distance at which shadows are rendered for this light. (Not used for ambient lights)*/
};