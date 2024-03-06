#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include <unordered_map>

#include "component_manager.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "matrix_4.hpp"
#include "default_components.hpp"

int main(int, char**) {
	Engine e;
	auto& thread_manager = e.getThreadManager();
	auto& component_manager = e.getComponentManager();
	auto& resourceM = e.getResourceManager();

	auto maybe_w = Window::create(e, 1024, 768, "Test Window", true);
	if (!maybe_w) return -1;
	
	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.enableCulling(true);
	w.enableDepthTest(true);
	w.setDepthTestMode(DepthTestMode::kLess);
	w.setCullingMode(CullingMode::kFront, FrontFace::kClockWise);

	Renderer renderer(e, w);

	auto color_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/Color.vs", "../assets/BasicShader/BasicColor/Color.fs");

	resourceM.LoadObj(e, "Cube", "../assets/obj_test.obj");
	resourceM.WaitResources();

	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Cube"), "CubeVertices", "CubeIndices");

	Texture laboon(TextureTarget::kTexture_2D, TextureFormat::kRGBA, TextureType::kUnsignedByte);
	unsigned laboon_handle = laboon.LoadTexture("../assets/laboon/laboon.png");

	//Cube 1
	size_t cube1 = component_manager.add_entity(TransformComponent(Vec3(-2.0f, 0.0f, -6.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		RenderComponent("Cube 1", "CubeVertices", "CubeIndices", color_shader, laboon_handle),
		ColorComponent(Vec4(0.5f, 0.5f, 0.75f, 1.0f)),
		BoxColliderComponent(Vec3(1.0f, 1.0f, 1.0f)));

	//Cube 2
	 size_t cube2 = component_manager.add_entity(TransformComponent(Vec3(2.0f, 0.0f, -6.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		 RenderComponent("Cube 2", "CubeVertices", "CubeIndices", color_shader, laboon_handle),
		 ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)),
		 BoxColliderComponent(Vec3(1.0f, 1.0f, 1.0f)));

  //Camera
	size_t main_camera = component_manager.add_entity(CameraComponent());
	w.setCurrentCam(main_camera);

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		Vec3 input;
		float rotate = 1;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		auto transform1 = component_manager.get_component<TransformComponent>(cube1);

		if (input_map.IsKeyPressed('W')) {
			transform1->pos_.y += input_velocity;
		}

		if (input_map.IsKeyPressed('S')) {
			transform1->pos_.y -= input_velocity;
		}

		if (input_map.IsKeyPressed('A')) {
			transform1->pos_.x -= input_velocity;
		}

		if (input_map.IsKeyPressed('D')) {
			transform1->pos_.x += input_velocity;
		}

		if (input_map.IsKeyPressed('Q')) {
			transform1->pos_.z -= input_velocity;
		}

		if (input_map.IsKeyPressed('E')) {
			transform1->pos_.z += input_velocity;
		}

		if (are_colliding_system(e, cube1, cube2)) {
			change_color_system(e, cube2, 1.0f, 0.2f, 0.2f, 1.0f);

			auto transform2 = component_manager.get_component<TransformComponent>(cube2);

			float cube_speed = 0.01f;

			Vec3 movement_vector = transform2->pos_ - transform1->pos_;

			movement_vector.Normalize();

			movement_vector *= cube_speed;

			transform2->pos_ += movement_vector;




		}
		else {
			change_color_system(e, cube2, 0.8f, 0.8f, 0.8f, 1.0f);
		}
		
		move_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input);
		rotate_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input_map, 1024, 768);
		imgui_transform_system(e, w);

		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}