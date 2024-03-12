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
#include "resource_manager.hpp"

//In this example we will se how to add an use lights
int main(int, char**) {

	Engine e;

	auto& thread_manager = e.getThreadManager();
	auto& component_manager = e.getComponentManager();
	auto& resourceM = e.getResourceManager();
	auto& cameraM = e.getCameraManager();

	auto maybe_w = Window::create(e, 1024, 768, "Test Window", true);
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);
	w.enableCulling(true);
	w.enableDepthTest(true);
	w.setDepthTestMode(DepthTestMode::kLess);
	w.setCullingMode(CullingMode::kFront, FrontFace::kClockWise);

	Renderer renderer(e, w);

	resourceM.LoadObj(e, "CubeOBJ", "../assets/obj_test.obj");

	//[1]. Add a shader that uses light (This is a basic shader already present on the engine)
	auto color_light_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/ColorLight.vs", "../assets/BasicShader/BasicColor/ColorLight.fs");


	resourceM.WaitResources();

	resourceM.createBuffersWithGeometry(resourceM.getGeometry("CubeOBJ"), "CubeVertices", "CubeIndices");

	//[2]. Add an entity that uses the shader loaded before
	size_t new_e = component_manager.add_entity(TransformComponent(Vec3(-50.0f, 30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
																							RenderComponent("Cube1", "CubeVertices", "CubeIndices", color_light_shader, 0), 
																							ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, 30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube2", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	new_e = component_manager.add_entity(TransformComponent(Vec3(50.0f, 30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube3", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	new_e = component_manager.add_entity(TransformComponent(Vec3(-50.0f, -30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube4", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube5", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	new_e = component_manager.add_entity(TransformComponent(Vec3(50.0f, -30.0f, -100.0f), Vec3(0.0f, 0.785f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube6", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.8f, 0.8f, 0.8f, 1.0f)));



	//[3]. Add lights to the scene (In this example we have all different 4 lights in different colors)
	size_t light_entity[4];

	//Ambient Light (White)
	light_entity[0] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.33f, 0.33f, 0.33f)));

	//Directional Light (Green)
	light_entity[1] = component_manager.add_entity(TransformComponent(Vec3(0.0f, -75.0f, -120.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.6f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));

	//Point Light (Red)
	light_entity[2] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, -120.0f), Vec3(0.0f, 60.0f, 75.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.007f, 0.0002f));

	//Spot Light (Turquoise)
	light_entity[3] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, -30.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.8f, 0.8f), Vec3(0.0f, 0.8f, 0.8f), 1.0f, 0.0014f, 0.000007f, 0.75f));


	//Camera
	size_t camera = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f)), CameraComponent("Camera", 1.0f, 1.0f));

	//Input Declaration
	Input input_map(w);
	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;
	float spot_speed = 1.0f;
	float spot_pos_value = 0.0f;
	float spot_spreading = 15.0f;

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
		w.calculateLastTime();

		input_map.updateInputs();
		w.updateImGui();

		Vec3 input;
		float rotate = 1;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input.z = input_velocity;
		}

		if (input_map.IsKeyPressed('S')) {
			input.z = -input_velocity;
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

		//SpotLight Movement
		spot_pos_value += w.getDeltaTime() * spot_speed;
		float movement_offset = cos(spot_pos_value) * spot_spreading;
		auto spot_tr = component_manager.get_component<TransformComponent>(light_entity[3]);
		spot_tr->pos_.x = 0.0f + movement_offset;


		cameraM.mouseRotate(input_map, 1024, 768);
		cameraM.move(input);
		imgui_transform_system(e, w);

		//[4]. Render
		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}