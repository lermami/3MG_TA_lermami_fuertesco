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

#include "component_manager.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "shader_management.hpp"
#include "buffer.hpp"
#include "thread_manager.hpp"
#include "default_systems.hpp"
#include "camera.hpp"

#include "matrix_4.hpp"


//In this exaple we will see how to add multiple geometries to the scene
int main(int, char**) {

	//[1]. Init the engine
	Engine e;

	//[2]. Get component and resource manager
	auto& thread_manager = e.getThreadManager();
	auto& component_manager = e.getComponentManager();
	auto& resourceM = e.getResourceManager();
	auto& cameraM = e.getCameraManager();

	auto maybe_w = Window::create(e, 1024, 768, "Test Window", false);
	if (!maybe_w) return -1;

	auto& w = maybe_w.value();
	w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

	Renderer renderer(e, w);

	//Create n triangles in random position
	int n_triangles = 10000;

	Geometry triangleGeo;

	//[3]. Create and init the geometries it could be 2D like this example or 3D (Values are on screen coordinates e.g. -1 t +1)
	triangleGeo.vertex_ = {
		{{-0.05f, -0.05f, 0}, {1, 0, 0}, {1, 0}, {1, 0, 0, 0}},
		{{0.05f, -0.05f, 0}, {0, 1, 0}, {1, 0}, {0, 1, 0, 0} },
		{{0.0f, 0.05f, 0}, {0, 0, 1}, {1, 0}, {0, 0, 1, 0} },
	};

	//[4]. Set the index of the triangles that compose the geometry
	triangleGeo.indices_ = { 0, 1, 2 };

	//[5]. Create a buffer to store the geometry
	resourceM.createBuffersWithGeometry(&triangleGeo, "TriangleVertices", "TriangleIndices");

	//[6].  Create a program for the geometry (This Shader is one of the basic shaders present on the engine) this shaders uses the color of the vertex
	auto simpleProgram = CreateProgram(w, "../assets/BasicShader/BasicColor/VertexColor.vs", "../assets/BasicShader/BasicColor/VertexColor.fs");

	for (int i = 0; i < n_triangles; i++) {
		Vec3 tr_pos;
		tr_pos.x = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.y = (float)((rand() % 200) - 100) / 100.0f;
		tr_pos.z = 0.0f;

		Vec3 tr_size(0.3f, 0.3f, 0.0f);
		Vec3 tr_rot(0.0f, 0.0f, 0.0f);

		//[7]. Add the entities that use the geometres you have created with your program
		size_t new_e = component_manager.add_entity(TransformComponent(Vec3(((rand() % 200) - 100) / 100.0f, ((rand() % 200) - 100) / 100.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f)),
			RenderComponent("Triangle", "TriangleVertices", "TriangleIndices", simpleProgram, 0));
	}

	//Input Declaration
	Input input_map(w);

	double mouse_x = 0, mouse_y = 0;
	size_t clicked_e = 0;

	while (!w.is_done() && !input_map.IsKeyDown(kKey_Escape)) {
		w.calculateLastTime();

		w.updateImGui();
		input_map.updateInputs();

		float input_x = 0, input_y = 0;
		float rotate = 0;
		double mouse_x = 0, mouse_y = 0;
		float input_velocity = 1.0f * w.getDeltaTime();

		input_map.getMousePos(mouse_x, mouse_y);

		if (input_map.IsKeyPressed('W')) {
			input_y = input_velocity;
		}
		
		if (input_map.IsKeyPressed('S')) {
			input_y = -input_velocity;
		}
		if (input_map.IsKeyPressed('A')) {
			input_x = -input_velocity;
		}

		if (input_map.IsKeyPressed('D')) {
			input_x = input_velocity;
		}

		if (input_map.IsKeyPressed('E')) {
			rotate = -input_velocity * 2.0f;
		}

		if (input_map.IsKeyPressed('Q')) {
			rotate = input_velocity * 2.0f;
		}
		
		if (input_map.IsKeyDown(kKey_LeftClick)) {
			clicked_e = on_click_system(*component_manager.get_component_list<TransformComponent>(), (float)mouse_x, (float)mouse_y);
		}

		if (input_map.IsKeyUp(kKey_LeftClick)) {
			clicked_e = 0;
		}

		if (clicked_e != 0)
			set_position_system(*component_manager.get_component<TransformComponent>(clicked_e), Vec3((float)mouse_x, (float)mouse_y, 0.0f));
		move_system(*component_manager.get_component_list<TransformComponent>(), Vec3(input_x, input_y, 0));
		rotate_system(*component_manager.get_component_list<TransformComponent>(), Vec3(0.0f, rotate, 0.0f));

		imgui_interface_system(e, w);

		//[8]. Render geometries
		renderer.render();

		w.swap();
	}

	return 0;

}