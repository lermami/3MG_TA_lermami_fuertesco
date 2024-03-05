#include<vector>
#include<optional>
#include<cassert>
#include<tuple>
#include<typeinfo>
#include<stdexcept>
#include<fstream>
#include<iostream>

#include "Window.hpp"
#include "Renderer.hpp"
#include "component_manager.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "camera.hpp"

#include "matrix_4.hpp"

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

	unsigned n_obj = 1000;

	auto simpleProgram = CreateProgram(w, "../assets/test_shader/test.vs", "../assets/test_shader/test.fs");

	resourceM.LoadObj(e, "Suzanne", "../assets/Suzanne.obj");
	resourceM.LoadObj(e, "Wolf", "../assets/wolf/Wolf_obj.obj");
	resourceM.LoadObj(e, "Gun", "../assets/gun/Gun.obj");
	resourceM.WaitResources();

	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Suzanne"), "SuzanneVertices", "SuzanneIndices");
	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Wolf"), "WolfVertices", "WolfIndices");
	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Gun"), "TankVertices", "TankIndices");

	for (unsigned i = 0; i < n_obj / 3; i++) {
		size_t new_e = component_manager.add_entity(TransformComponent(Vec3((rand() % 150) - 75, (rand() % 150) - 75, -100.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f)),
			RenderComponent("Suzanne", "SuzanneVertices", "SuzanneIndices", simpleProgram, 0));
	}

	for (unsigned i = n_obj / 3; i < 2 * n_obj / 3; i++) {
		size_t new_e = component_manager.add_entity(TransformComponent(Vec3((rand() % 28) - 14, (rand() % 28) - 14, -20.0f), Vec3(0.0f,  0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f)),
			RenderComponent("Wolf", "WolfVertices", "WolfIndices", simpleProgram, 0));
	}

	for (unsigned i = 2 * n_obj / 3; i < n_obj; i++) {
		size_t new_e = component_manager.add_entity(TransformComponent(Vec3((rand() % 100) - 50, (rand() % 100) - 50, -60.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f)),
			RenderComponent("Tank", "TankVertices", "TankIndices", simpleProgram, 0));
	}

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	//Camera
	size_t main_camera = component_manager.add_entity(CameraComponent());
	w.setCurrentCam(main_camera);

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		Vec3 input;
		float rotate = 0;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input.z = input_velocity * 10.0f;
		}

		if (input_map.IsKeyPressed('S')) {
			input.z = -input_velocity * 10.0f;
		}

		if (input_map.IsKeyPressed('A')) {
			input.x = -input_velocity;
		}

		if (input_map.IsKeyPressed('D')) {
			input.x = input_velocity;
		}

		if (input_map.IsKeyPressed('Q')) {
			input.y = -input_velocity;
		}

		if (input_map.IsKeyPressed('E')) {
			input.y = input_velocity;
		}

		if (input_map.IsKeyPressed(kKey_Right)) {
			rotate = -input_velocity * 2.0f;
		}

		if (input_map.IsKeyPressed(kKey_Left)) {
			rotate = input_velocity * 2.0f;
		}

		if (input_map.IsKeyDown(kKey_LeftClick)) {
			//clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (input_map.IsKeyUp(kKey_LeftClick)) {
			clicked_e = 0;
		}

		//if (clicked_e != 0)
			//set_position_system(*component_manager.get_component<TransformComponent>(clicked_e), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		//move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, input_z));

		move_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input);
		rotate_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input_map, 1024, 768);
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));
		imgui_transform_system(e, w);

		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}