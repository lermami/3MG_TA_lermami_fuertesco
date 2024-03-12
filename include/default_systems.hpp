/**
 * @file default_systems.hpp
 * @brief Header file for the systems of the components.
 *
 * @defgroup Default Systems Default Systems
 * @brief This file defines the default systems of entities.
 */
#pragma once
#include "component_manager.hpp"

class Camera;
class VertexBuffer;
class Input;

/**
 * @brief Changes the color of a component in the engine.
 *
 * @param e Reference to the Engine object.
 * @param entity ID of the entity to modify.
 * @param r Red component (0.0f to 1.0f).
 * @param g Green component (0.0f to 1.0f).
 * @param b Blue component (0.0f to 1.0f).
 * @param a Alpha component (transparency, default 1.0f).
 */
void change_color_system(Engine& e, size_t entity, float r, float g, float b, float a = 1.0f);

/**
 * @brief Moves all provided TransformComponents by a specified offset.
 *
 * @param transforms List of TransformComponents to move.
 * @param mov Movement offset to apply.
 */
void move_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 mov);

/**
 * @brief Rotates all provided TransformComponents by a specified amount.
 *
 * @param transforms List of TransformComponents to rotate.
 * @param rot Rotation amount to apply.
 */
void rotate_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 rot);

/**
 * @brief Sets the position, rotation, and scale of a specific entity's TransformComponent.
 *
 * @param e Reference to the Engine object.
 * @param entity ID of the entity to modify.
 * @param position New position for the entity.
 * @param rotation New rotation for the entity.
 * @param size New scale for the entity.
 */
void set_transform_system(Engine& e, size_t entity, Vec3 position, Vec3 rotation, Vec3 size);

/**
 * @brief Checks for clicks on any entity's collider and returns the clicked entity's ID.
 *
 * @param transforms List of TransformComponents (likely containing colliders).
 * @param mouse_x X-coordinate of the mouse click.
 * @param mouse_y Y-coordinate of the mouse click.
 * @return ID of the clicked entity (or -1 if no entity was clicked).
 */
size_t on_click_system(std::vector<std::optional<TransformComponent>>& transforms, float mouse_x, float mouse_y);

/**
 * @brief Sets the position of a TransformComponent.
 *
 * @param transform Reference to the TransformComponent to modify.
 * @param pos New position for the component.
 */
void set_position_system(TransformComponent& transform, Vec3 pos);

/**
 * @brief Updates and plays all AudioComponents in a provided list.
 *
 * @param audio_list List of AudioComponents to update and play.
 */
void basic_sound_system(std::vector<std::optional<AudioComponent>>& audio_list);

/**
 * @brief Checks for collision between two entities using their colliders.
 *
 * @param e Reference to the Engine object.
 * @param entity1 ID of the first entity to check for collision.
 * @param entity2 ID of the second entity to check for collision.
 * @return True if the entities are colliding, false otherwise.
 */
bool are_colliding_system(Engine& e, size_t entity1, size_t entity2);

/**
 * @brief Integrates ImGui for editing an entity's transform component within the engine window.
 *
 * @param e Reference to the Engine object.
 * @param w Reference to the Window object representing the engine window.
 */
void imgui_transform_system(Engine& e, Window& w);