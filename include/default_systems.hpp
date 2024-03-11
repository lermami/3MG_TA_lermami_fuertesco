/**
 * @file defaul_systems.hpp
 * @brief Header file for the systems of the components.
 *
 * @defgroup defaul_systems defaul_systems
 * @brief This file defines the defaul systems of entities.
 */
#pragma once
#include "component_manager.hpp"

class Camera;
class VertexBuffer;
class Input;

/**
 * @brief Initializes a RenderComponent with various properties.
 *
 * @param render Reference to the RenderComponent to initialize.
 * @param name Name for the render component.
 * @param vertexBuffer Name or handle of the vertex buffer to use.
 * @param orderBuffer Name or handle of the index buffer.
 * @param program Handle of the shader program to use for rendering.
 * @param texture Handle of the texture to use for rendering (optional).
 */
void init_render_component_system(RenderComponent& render, const char* name, std::string vertexBuffer, std::string orderBuffer, unsigned int program, unsigned int texture);

/**
 * @brief Initializes a TransformComponent with initial position, rotation, and scale.
 *
 * @param transform Reference to the TransformComponent to initialize.
 * @param pos Initial position of the entity.
 * @param rot Initial rotation of the entity.
 * @param size Initial scale of the entity.
 */
void init_transform_system(TransformComponent& transform, Vec3& pos, Vec3& rot, Vec3& size);

/**
 * @brief Initializes an AudioComponent with a sound buffer, playback configuration, and optional label.
 *
 * @param audio Reference to the AudioComponent to initialize.
 * @param buff Reference to the SoundBuffer to use.
 * @param label Optional label for the audio component.
 * @param pos Position of the audio source in world space.
 * @param vel Velocity of the audio source.
 * @param gain Gain (volume) of the audio source (default 1.0f).
 * @param pitch Pitch of the audio playback (default 1.0f).
 * @param playing Whether the audio should start playing immediately (default false).
 */
void init_audio_system(AudioComponent& audio, SoundBuffer& buff, const char* label, ALfloat pos[3], ALfloat vel[3], float gain = 1.0f, float pitch = 1.0f, bool playing = false);

/**
 * @brief Initializes a ColorComponent with a base color and optional alpha.
 *
 * @param color Reference to the ColorComponent to initialize.
 * @param r Red component (0.0f to 1.0f).
 * @param g Green component (0.0f to 1.0f).
 * @param b Blue component (0.0f to 1.0f).
 * @param a Alpha component (transparency, default 1.0f).
 */
void init_color_system(ColorComponent& color, float r, float g, float b, float a = 1.0f);

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
 * @brief Initializes a LightComponent as an ambient light source with a specific color.
 *
 * @param light Reference to the LightComponent to initialize.
 * @param color Color of the ambient light.
 */
void init_ambient_light_system(LightComponent& light, Vec3 color);

/**
 * @brief Initializes a LightComponent as a directional light source with direction, color, and specular component.
 *
 * @param light Reference to the LightComponent to initialize.
 * @param direction Direction of the light.
 * @param color Color of the light.
 * @param specular Specular color of the light (for shininess effects).
 */
void init_directional_light_system(LightComponent& light, Vec3 direction, Vec3 color, Vec3 specular);

/**
 * @brief Initializes a LightComponent as a point light source with color, specular component, and attenuation factors.
 *
 * @param light Reference to the LightComponent to initialize.
 * @param color Color of the light.
 * @param specular Specular color of the light (for shininess effects).
 * @param constant Constant attenuation factor (affecting all distances).
 * @param linear Linear attenuation factor (increasing with distance).
 * @param quadratic Quadratic attenuation factor (increasing faster with distance).
 */
void init_point_light_system(LightComponent& light, Vec3 color, Vec3 specular, float constant, float linear, float quadratic);

/**
 * @brief Initializes a LightComponent as a spot light source with direction, color, specular component, attenuation factors, and cutoff angle.
 *
 * @param light Reference to the LightComponent to initialize.
 * @param direction Direction of the light.
 * @param color Color of the light.
 * @param specular Specular color of the light (for shininess effects).
 * @param constant Constant attenuation factor (affecting all distances).
 * @param linear Linear attenuation factor (increasing with distance).
 * @param quadratic Quadratic attenuation factor (increasing faster with distance).
 * @param cutoff_angle Angle defining the cone of light falloff.
 */
void init_spot_light_system(LightComponent& light, Vec3 direction, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle);

/**
 * @brief Initializes a CameraComponent with position, movement speed, and rotation sensitivity.
 *
 * @param cameraComp Reference to the CameraComponent to initialize.
 * @param pos Initial position of the camera. (default: Vec3(0.0f, 0.0f, 0.0f))
 * @param speed Movement speed of the camera. (default: 1.0f)
 * @param sensitivity Rotation sensitivity of the camera. (default: 1.0f)
 */
void init_camera_system(CameraComponent& cameraComp, Vec3 pos = Vec3(0.0f, 0.0f, 0.0f), float speed = 1.0f, float sensitivity = 1.0f);

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
 * @brief Moves a CameraComponent based on provided input vector.
 *
 * @param cam Reference to the CameraComponent to move.
 * @param input Input vector representing movement direction.
 */
void move_camera_system(CameraComponent& cam, Vec3 input);

/**
 * @brief Rotates a CameraComponent based on provided input and screen dimensions.
 *
 * @param cam Reference to the CameraComponent to rotate.
 * @param input Reference to the Input object for reading mouse movement.
 * @param w Width of the screen.
 * @param h Height of the screen.
 */
void rotate_camera_system(CameraComponent& cam, Input& input, const float w, const float h);

/**
 * @brief Updates and plays all AudioComponents in a provided list.
 *
 * @param audio_list List of AudioComponents to update and play.
 */
void basic_sound_system(std::vector<std::optional<AudioComponent>>& audio_list);

/**
 * @brief Initializes a BoxColliderComponent with a specific size and optional center offset.
 *
 * @param component Reference to the BoxColliderComponent to initialize.
 * @param extent Half-extents of the box collider (defining its size).
 * @param center_offset Optional offset from the transform's center for the collider's position. (default: Vec3(0.0f, 0.0f, 0.0f))
 */
void init_box_collider_system(BoxColliderComponent& component, Vec3 extent, Vec3 center_offset = Vec3(0.0f, 0.0f, 0.0f));

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