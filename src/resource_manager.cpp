#include "resource_manager.hpp"
#include <tiny_obj_loader.h>
#include "default_components.hpp"
#include "buffer.hpp"

#include "Engine.hpp"
#include "thread_manager.hpp"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
  //TODO: handle 
	for (auto& [name, buffer] : vertexBuffers_) {
		delete buffer;
	}
}

void ResourceManager::loadTexture(const char* name, Texture tex, const char* path) {
	textures_[name] = tex.LoadTexture(path);
}

void ResourceManager::LoadObj(Engine& e, const char* name, const char* path) {
	auto& threadM = e.getThreadManager();

	std::function<Geometry()> mycall_vertex = [this, name, path]() { return LoadObj(name, path); };
	std::future<Geometry> future = std::move(threadM.add(mycall_vertex));
	geometryFutures_[name] = std::move(future);
}

void ResourceManager::WaitResources() {
	for (auto& geometry : geometryFutures_) {
		if (geometry.second.valid()) {
			geometries_[geometry.first] = geometry.second.get();
		}
	}
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

	//geometries_.push_back(geometry);
	//geometry_names_.push_back(name);

	return geometry;
}

Geometry* ResourceManager::getGeometry(std::string nameID) {
	if (geometries_.count(nameID) == 0) {
		printf("ERROR. VERTEX BUFFER '%s' NOT FOUND", nameID.c_str());
		return nullptr;
	}

	return &geometries_.at(nameID);
}

unsigned ResourceManager::getTexture(const char* name) {
	if (textures_.count(name) == 0) {
		printf("ERROR. VERTEX BUFFER '%s' NOT FOUND", name);
		return 0;
	}

	return textures_.at(name);
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

bool ResourceManager::createIndexBuffer(std::string nameID, unsigned* indices, unsigned size) {
	if (indexBuffers_.count(nameID) > 0) {
		printf("ERROR. INDEX BUFFER NAMED '%s' ALREADY EXISTS", nameID.c_str());
		return false;
	}

	indexBuffers_[nameID] = new IndexBuffer(indices, size);

	return true;
}

IndexBuffer* ResourceManager::getIndexBuffer(std::string nameID) {
	if (indexBuffers_.count(nameID) == 0) {
		printf("ERROR. INDEX BUFFER '%s' NOT FOUND", nameID.c_str());
		return nullptr;
	}

	return indexBuffers_.at(nameID);
}

bool ResourceManager::createBuffersWithGeometry(Geometry* geo, std::string nameIDVertex, std::string nameIDIndex) {
	if (vertexBuffers_.count(nameIDVertex) > 0) {
		printf("ERROR. VERTEX BUFFER NAMED '%s' ALREADY EXISTS", nameIDVertex.c_str());
		return false;
	}

	if (indexBuffers_.count(nameIDIndex) > 0) {
		printf("ERROR. INDEX BUFFER NAMED '%s' ALREADY EXISTS", nameIDIndex.c_str());
		return false;
	}


	vertexBuffers_[nameIDVertex] = new VertexBuffer(&geo->vertex_[0].pos.x, geo->vertex_.size() * sizeof(Vertex));
	indexBuffers_[nameIDIndex] = new IndexBuffer(&geo->indices_[0], geo->indices_.size() * sizeof(unsigned));

	return true;
}