#pragma once
#include "component_manager.hpp"

class Camera;

void init_render_component_system(RenderComponent& render, Geometry& geometry, unsigned int program, unsigned int texture);

void init_transform_system(TransformComponent& transform, Vec3& pos, Vec3& rot, Vec3& size);

void init_audio_system(AudioComponent& audio, SoundBuffer& buff, const char* label, ALfloat pos[3], ALfloat vel[3], float gain = 1.0f, float pitch = 1.0f, bool playing = false);

void init_color_system(RenderComponent& render, float r, float g, float b, float a);

void init_ambient_light_system(LightComponent& light, Vec3 color);

void init_directional_light_system(LightComponent& light, Vec3 direction, Vec3 color, Vec3 specular);

void init_point_light_system(LightComponent& light, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic);

void init_spot_light_system(LightComponent& light, Vec3 direction, Vec3 position, Vec3 color, Vec3 specular, float constant, float linear, float quadratic, float cutoff_angle);

void init_camera_system(CameraComponent& cameraComp, Vec3 pos = Vec3(0.0f, 0.0f, 0.0f), float speed = 1.0f, float sensitivity = 1.0f);

void move_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 mov);

void rotate_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 rot);

size_t on_click_system(std::vector<std::optional<TransformComponent>>& transforms, float mouse_x, float mouse_y);

void set_position_system(TransformComponent& transform, Vec3 pos);

void move_camera_system(CameraComponent& cam, Vec3 input);

void rotate_camera_system(CameraComponent& cam, Input& input, const float w, const float h);

void shader_prop_system(std::vector<std::optional<RenderComponent>>& renders, std::vector<std::optional<TransformComponent>>& transforms);

void render_system(Window& w, CameraComponent& current_cam, std::vector<std::optional<RenderComponent>>& renders, std::vector<std::optional<TransformComponent>>& transforms, std::vector<std::optional<LightComponent>>& lights);

void basic_sound_system(std::vector<std::optional<AudioComponent>>& audio_list);

void imgui_transform_system(TransformComponent& transform);