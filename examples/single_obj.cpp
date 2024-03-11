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

//In this example we will se how to add an object to the engine
int main(int, char**) {

	//[1]. Init the engine
	Engine e;

	//[2]. Get component and resource and thread manager
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

	//[3]. Load OBJ
	resourceM.LoadObj(e, "LaboonObj", "../assets/laboon/laboon.obj");
	resourceM.LoadObj(e, "CubeObj", "../assets/obj_test.obj");
	resourceM.LoadObj(e, "SuzanneObj", "../assets/Suzanne.obj");

	//[4]. Load obj textures if necesary
	resourceM.loadTexture("Laboon", Texture(TextureTarget::kTexture_2D, TextureFormat::kRGBA, TextureType::kUnsignedByte),
																						 "../assets/laboon/laboon.png");

	resourceM.loadTexture("Bricks", Texture(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte),
																						"../assets/wall.jpg");

	//[5]. Add a shader for the object 
	auto texture_light_shader = CreateProgram(w, "../assets/BasicShader/Texture/TextureLight.vs", "../assets/BasicShader/Texture/TextureLight.fs");


	//[6]. Wait for resourcess to load
	resourceM.WaitResources();

	//[7]. Set a buffer to store your geometry
	resourceM.createBuffersWithGeometry(resourceM.getGeometry("LaboonObj"), "LaboonVertices", "LaboonIndices");

	//[8]. Add an entity of your geometry
	size_t new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -75.0f, -200.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		                                          RenderComponent("Laboon", "LaboonVertices", "LaboonIndices", texture_light_shader, resourceM.getTexture("Laboon")));
	
  //Light
	size_t light_entity[4];

		//Ambient Light
	light_entity[0] = component_manager.add_entity(TransformComponent(Vec3(0.0f, -75.0f, -200.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		                                             LightComponent(Vec3(0.33f, 0.33f, 0.33f)));
	
		//Directional Light
	light_entity[1] = component_manager.add_entity(TransformComponent(Vec3(0.0f, -75.0f, -120.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
																								LightComponent(Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));

		//Point Light
	light_entity[2] = component_manager.add_entity(TransformComponent(Vec3(0.0f, 25.0f, -100.0f), Vec3(0.0f, 60.0f, 75.0f), Vec3(1.0f, 1.0f, 1.0f)),
																							 	 LightComponent(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.007f, 0.0002f));

		//Spot Light
	light_entity[3] = component_manager.add_entity(TransformComponent(Vec3(50.0f, 0.0f, -100.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
																								LightComponent(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), 1.0f, 0.0014f, 0.000007f, 0.75f));


  //Camera
	size_t main_camera = component_manager.add_entity(TransformComponent(), CameraComponent());
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

		//[9]. Render
		renderer.render();

		w.swap();

		w.calculateCurrentTime();
	}

	return 0;

}