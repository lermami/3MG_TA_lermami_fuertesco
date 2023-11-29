#pragma once
#include "component_manager.hpp"

void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v,
	std::vector<unsigned>& indices_, unsigned int program);

void init_transform_system(TransformComponent& transform, Vec3& pos, Vec3& rot, Vec3& size);

void init_color_system(RenderComponent& render, float r, float g, float b, float a);

void move_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 mov);

void rotate_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 rot);

size_t on_click_system(std::vector<std::optional<TransformComponent>>& transforms, float mouse_x, float mouse_y);

void set_position_system(TransformComponent& transform, Vec3 pos);

void render_system(std::vector<std::optional<RenderComponent>>& renders, std::vector<std::optional<TransformComponent>>& transforms);