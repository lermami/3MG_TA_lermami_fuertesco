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
#include "Renderer.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "camera.hpp"

using namespace std::chrono_literals;

#include "matrix_4.hpp"

void check_GL(const std::string& text, const char* file, int line) {
	if (auto err = glGetError(); err != GL_NO_ERROR) {
		printf("%s: %s(%d) %s\n", text.c_str(), file, line, gluErrorString(err));
	}
}

#define CHECKGL(s) check_GL(s,__FILE__,__LINE__)

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

	auto simpleProgram = CreateProgram(w, "../assets/laboon/laboon.vs", "../assets/laboon/laboon.fs");
	auto simpleProgram3 = CreateProgram(w, "../assets/Shader/ShadowMap/depthtest.vs", "../assets/Shader/ShadowMap/depthtest.fs");
	auto texture_light_shadow = CreateProgram(w, "../assets/BasicShader/Texture/TextureLightShadow.vs", "../assets/BasicShader/Texture/TextureLightShadow.fs");

	resourceM.LoadObj(e, "Square", "../assets/square.obj");
	resourceM.LoadObj(e, "Cube", "../assets/obj_test.obj");
	resourceM.WaitResources();

	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Square"), "SquareVertices", "SquareIndices");
	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Cube"), "CubeVertices", "CubeIndices");


	unsigned n_obj = 1000;
	
	Texture laboon(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte);
	unsigned wall_texture = laboon.LoadTexture("../assets/wall.jpg");

	//Cubes

		//Cube 1
	size_t new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -75.0f, -300.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(100.0f, 100.0f, 5.0f)),
		RenderComponent("Cube 1", "CubeVertices", "CubeIndices", texture_light_shadow, wall_texture));
	
		//Cube 2
	new_e = component_manager.add_entity(TransformComponent(Vec3(25.0f, -20.0f, -250.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(18.0f, 18.0f, 18.0f)),
		RenderComponent("Cube 2", "CubeVertices", "CubeIndices", texture_light_shadow, wall_texture));

	//Cube 3
	new_e = component_manager.add_entity(TransformComponent(Vec3(20.0f, 10.0f, -190.0f), Vec3(0.0f, 0.0f, 1.57f), Vec3(20.0f, 10.0f, 20.0f)),
		RenderComponent("Cube 3", "CubeVertices", "CubeIndices", texture_light_shadow, wall_texture));

	//Floor
	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -3.0f, -350.0), Vec3(1.57f, 0.0f, 0.0f), Vec3(200.0f, 100.0f, 200.0f)),
		RenderComponent("Wall", "SquareVertices", "SquareIndices", texture_light_shadow, wall_texture));

  //Light
	size_t light_entity[2];
		
	//Directional
	light_entity[0] = component_manager.add_entity(TransformComponent(Vec3(-20.0f, 0.0f, 80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.1f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)));

		//Ambient
	light_entity[1] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.5f, 0.5f, 0.5f)));
  
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
		
		move_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input);
		rotate_camera_system(*component_manager.get_component<CameraComponent>(main_camera), input_map, 1024, 768);

		imgui_transform_system(e, w);
		
		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}