#include "Engine.hpp"
#include <tiny_obj_loader.h>
#include "GLFW/glfw3.h"


Engine::Engine(){
  glfwInit();

	srand(time(NULL));
}

Engine::~Engine(){
  glfwTerminate();
}

Geometry Engine::LoadObj(const char* path) {
	Geometry geometry;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string warning, error;

	bool err = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);
	int id = 0;

	if (!err) {
		if (!error.empty()) {
			printf("Error loading obj: %s", error.c_str());
		}
	}
	/*
	if (!warning.empty()) {
		std::cout << "Warning loading obj: " << warning.c_str();
	}*/

	std::map<int, Vertex> map;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex vertex;
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				vertex.pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				vertex.pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				vertex.pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					vertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					vertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					vertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					vertex.uv.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.uv.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// vertex.red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// vertex.green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// vertex.blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

				bool found = false;

				for (auto it = map.begin(); it != map.end() && !found; it++) {
					if (vertex == it->second) {
						geometry.indices_.push_back(it->first);
						found = true;
					}
				}

				if (!found) {
					map.emplace(id, vertex);
					geometry.vertex_.push_back(vertex);
					geometry.indices_.push_back(id);
					id++;
				}
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return geometry;
}

ComponentManager* Engine::getComponentManager() {
	return componentM_;
}