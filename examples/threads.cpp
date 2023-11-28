#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tiny_obj_loader.h>

#include <optional>
#include <string>
#include <vector>
#include <future>
#include <iostream>
#include <chrono>
#include <typeinfo>
using namespace std::chrono_literals;

#include "Window.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "geometry_test.hpp"
#include "shader_management.hpp"
#include "thread_manager.hpp"
#include "component_manager.hpp"

double heavyFunction(int id, int iterations) {
  double result = 0.0;
  for (int i = 0; i < iterations; ++i) {
    result += sin(i) * tan(i);
  }

  printf("a");

  return result;
}

int* CreateTexture(int w, int h) {
  int* t = new int[w*h]();

  for (int i = 0; i < w * h; ++i) {
    t[i] = i;
  }
  return t;
}

bool LoadObj(const char* path, std::vector<Vertex>& vertex, std::vector<unsigned>& indices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string warning, error;
	std::cout << "a";

	bool err = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);

	if (!err) {
		if (!error.empty()) {
			std::cout << "Error loading obj: " << error.c_str();
		}
	}

	if (!warning.empty()) {
		std::cout << "Warning loading obj: " << warning.c_str();
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;

		for (int i = 0; i < attrib.vertices.size() / 3; i++) {
			/*
			Vertex vx;
			vx.x_ = attrib.vertices[3 * i + 0];
			vx.y_ = attrib.vertices[3 * i + 1];
			vx.z_ = attrib.vertices[3 * i + 2];
			vertex.push_back(vx);
			*/
		}

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				indices.push_back(static_cast<unsigned int>(idx.vertex_index));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}

	}

	return true;
}

int main(int, char**) {
	Engine e;
  ThreadManager thread_manager;
	ComponentManager component_manager;

	std::future<bool> future;

	std::vector<Vertex> obj_test;
	std::vector<unsigned> obj_indices_test;

  std::function<bool()> mycall_double = [&obj_test, &obj_indices_test]() { return LoadObj("../include/Suzanne.obj", obj_test, obj_indices_test); };
  future = thread_manager.add(mycall_double);

  thread_manager.waitFuture(future);
  double num = future.get();
  std::cout << "Resultado: " << num << std::endl;
  
  /*
  std::string key = "Texture";
  std::function<double()> mycall_double = []() { return heavyFunction(0, 1000000); };
  std::future<double> future = thread_manager.addToChain(mycall_double, key);

  std::future<double> future2 = thread_manager.addToChain(mycall_double, key);

  */
  return 0;
}