#include "resource_manager.hpp"
#include <tiny_obj_loader.h>
#include "default_components.hpp"
#include "buffer.hpp"


ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
  //TODO: handle 
	for (auto& [name, buffer] : vertexBuffers_) {
		delete buffer;
	}
}

unsigned ResourceManager::loadTexture(const char* name, Texture tex, const char* path) {
  unsigned new_tex = tex.LoadTexture(path);

  textures_.push_back(new_tex);
  texture_names_.push_back(name);

  return new_tex;
}

Geometry ResourceManager::LoadObj(const char* name, const char* path) {
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

		}
	}

	geometries_.push_back(geometry);
	geometry_names_.push_back(name);

	return geometry;
}

unsigned ResourceManager::getTexture(const char* name) {
  int it = 0;
  for (auto& tex_name : texture_names_) {
    if (tex_name == name) {
      return textures_[it];
    }
    it++;
  }

  return 0;
}

unsigned ResourceManager::getTexture(int index) {
  if (index > textures_.size() || index < 0) return 0;

  return textures_[index];
}

std::string ResourceManager::getTextureName(unsigned value) {
  int it = 0;

  for (auto& tex_value : textures_) {
    if (tex_value == value) {
      return texture_names_[it];
    }

    it++;
  }
}

std::vector<unsigned>& ResourceManager::getTextureList(){
  return textures_;
}

std::vector<std::string>& ResourceManager::getTextureNamesList() {
  return texture_names_;
}

bool ResourceManager::createVertexBuffer(std::string nameID, float* vertices, unsigned size)
{
	if (vertexBuffers_.count(nameID) > 0) {
		printf("ERROR. VERTEX BUFFER NAMED '%s' ALREADY EXISTS", nameID.c_str());
		return false;
	}

	vertexBuffers_[nameID] = new VertexBuffer(vertices, size);

	return true;
}

VertexBuffer* ResourceManager::getVertexBuffer(std::string nameID) {
	if (vertexBuffers_.count(nameID) == 0) {
		printf("ERROR. VERTEX BUFFER '%s' NOT FOUND", nameID.c_str());
		return nullptr;
	}

	return vertexBuffers_.at(nameID);
}