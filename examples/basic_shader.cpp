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
#include "Renderer.hpp"
#include "Window.hpp"
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

//In this example we will see the different basic shaders that this engine has and the necessary steps to use them.
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

	//[1]. Create and Load the program variable, you can use your own or one of the basics that this engine already have.
	
	//These are some of the basic shaders present in the engine to make basic examples:

	//Only a single texture
	auto texture_shader = CreateProgram(w, "../assets/BasicShader/Texture/Texture.vs", "../assets/BasicShader/Texture/Texture.fs");

	//A single texture but interact with lights (Max 5 light of each type)
	auto texture_light_shader = CreateProgram(w, "../assets/BasicShader/Texture/TextureLight.vs", "../assets/BasicShader/Texture/TextureLight.fs");

	//A single texture but interact with lights and recieves shadows of a directional light
	auto texture_light_shadow_shader = CreateProgram(w, "../assets/BasicShader/Texture/TextureLightShadow.vs", "../assets/BasicShader/Texture/TextureLightshadow.fs");

	//Only a plane color
	auto color_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/Color.vs", "../assets/BasicShader/BasicColor/Color.fs");

	//A plane color but interact with lights (Max 5 light of each type)
	auto color_light_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/ColorLight.vs", "../assets/BasicShader/BasicColor/ColorLight.fs");

	//A plane color but interact with lights and recieves shadows of a directional light
	auto color_light_shadow_shader = CreateProgram(w, "../assets/BasicShader/BasicColor/ColorLightShadow.vs", "../assets/BasicShader/BasicColor/ColorLightshadow.fs");




	//Depth texture is sent to shader as "uniform sampler2D u_depth_map" and Texture is sent to shader as "uniform sampler2D u_texture"

	resourceM.LoadObj(e, "Cube", "../assets/obj_test.obj");
	resourceM.WaitResources();
	resourceM.createBuffersWithGeometry(resourceM.getGeometry("Cube"), "CubeVertices", "CubeIndices");

	unsigned n_obj = 1000;

	//[2]. (Skip this step if you dont use a texture in your shader) Load in the ResourceManager the texture that you are gona use in the shader.
	//The name used in loadTexture will be used to refer to the texture loaded. (Maximum one texture per shader).
	resourceM.loadTexture("Wall", Texture(TextureTarget::kTexture_2D, TextureFormat::kRGB, TextureType::kUnsignedByte),
		"../assets/wall.jpg");

	//[3]. On any render component add in the program variable that you have just created
	
	//In the case of basic engine shaders, for  textures ones you have to add the texture name in the getTexture function inside the RenderComponent constructor, and in the case of plane colours you have to add the ColorComponent as variable on add_entity function
	
	//Cubes
		//1
	size_t new_e = component_manager.add_entity(TransformComponent(Vec3(-30.0f, 20.0f, -80.0f), Vec3(0.0f, 1.57f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)), 
		RenderComponent("Cube 1", "CubeVertices", "CubeIndices", texture_shader, resourceM.getTexture("Wall")));

		//2
	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, 20.0f, -80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 2", "CubeVertices", "CubeIndices", texture_light_shader, resourceM.getTexture("Wall")));

		//3
	new_e = component_manager.add_entity(TransformComponent(Vec3(30.0f, 20.0f, -80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 3", "CubeVertices", "CubeIndices", texture_light_shadow_shader, resourceM.getTexture("Wall")));

		//4
	new_e = component_manager.add_entity(TransformComponent(Vec3(-30.0f, -20.0f, -80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 4", "CubeVertices", "CubeIndices", color_shader, 0),
		ColorComponent(Vec4(0.5f, 0.5f, 0.75f, 1.0f)));

		//5
	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, -20.0f, -80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 5", "CubeVertices", "CubeIndices", color_light_shader, 0),
		ColorComponent(Vec4(0.5f, 0.5f, 0.75f, 1.0f)));

		//6
	new_e = component_manager.add_entity(TransformComponent(Vec3(30.0f, -20.0f, -80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f)),
		RenderComponent("Cube 6", "CubeVertices", "CubeIndices", color_light_shadow_shader, 0),
		ColorComponent(Vec4(0.5f, 0.5f, 0.75f, 1.0f)));

	//Back Wall
	new_e = component_manager.add_entity(TransformComponent(Vec3(0.0f, 0.0f, -120.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(50.0f, 50.0f, 0.5f)),
		RenderComponent("Wall", "CubeVertices", "CubeIndices", texture_light_shadow_shader, resourceM.getTexture("Wall")),
		ColorComponent(Vec4(0.5f, 0.5f, 0.75f, 1.0f)));

	//Light
	size_t light_entity[2]; 

		//Directional
	light_entity[0] = component_manager.add_entity(TransformComponent(Vec3(10.0f, 0.0f, 80.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)),
		LightComponent(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 1.0f)));

		//Ambiental
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