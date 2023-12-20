#pragma once
#include<unordered_map>
#include<memory>
#include<optional>
#include<cassert>

#include "buffer.hpp"
#include "vector_3.hpp"
#include "matrix_4.hpp"
#include "sound/soundsource.h"

struct Vertex {
	float x_, y_, z_;
	float r_, g_, b_, a_;
	float nx_, ny_, nz_;
	float u_, v_;
};

struct TransformComponent {
	Mat4 model_matrix_;

	Vec3 pos_;
	Vec3 rot_;
	Vec3 size_;
};

struct AudioComponent {
	AudioComponent(std::string name, ALfloat pos[3],
		ALfloat speed[3], float gain = 1.0f, float pitch = 1.0f)
		: sound_source_(name, pos, speed, gain, pitch) {}

	AudioComponent() = default;
	AudioComponent& operator=(const AudioComponent& o) = default;

	SoundSource sound_source_;
};

struct RenderComponent {
	std::vector<Vertex> vertex_;
	std::vector<unsigned> indices_;
	std::shared_ptr<Buffer> elements_buffer_;
	std::shared_ptr<Buffer> order_buffer_;
	unsigned int program_ = -1;
};

struct component_base {
	virtual void add_component() = 0;
	virtual void add_component(int position) = 0;
	virtual void delete_component(size_t id) = 0;
	virtual size_t size() = 0;
};

template<typename T>
struct component_list : component_base {
	std::vector < std::optional<T>> components_;

	virtual void add_component() override {
		T a;
		components_.emplace_back(a);
	}

	virtual void add_component(int position) override {
		T a;
		//-1 because position refers to an entity that starts in 1
		components_[(size_t)position-1] = a;
	}

	virtual size_t size() override {
		return components_.size();
	}

	virtual void delete_component(size_t id) override {
		assert(id != 0);
		components_[id-1].reset();
		components_[id-1] = std::nullopt;
	}
};

struct ComponentManager {

	std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_classes_;
	std::vector <size_t> deleted_components_;

	ComponentManager() {
		add_component_class<RenderComponent>();
		add_component_class<TransformComponent>();
		add_component_class<AudioComponent>();
	}

	template<typename T> void add_component_class() {
		component_classes_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
	}

	template<typename T>
	T* get_component(size_t e) {
		assert(component_classes_.size());
		//assert(component_classes_.contains(typeid(T).hash_code()));
		assert(e != 0);
		//Entities start on 1
		e = e - 1;
		auto& component_l = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
		assert(e < component_l.size());
		auto& component_opt = component_l.components_.at(e);

		if (!component_opt.has_value()) return nullptr;
		return &component_opt.value();
	}
	
	template<typename T>
	std::vector < std::optional<T>>* get_component_list() {
		assert(component_classes_.size());
		auto& component_l = *static_cast<component_list<T>*>(component_classes_.find(typeid(T).hash_code())->second.get());
		auto& component_opt = component_l.components_;

		return &component_opt;
	}
	

	size_t add_entity();

	void remove_entity(size_t id);

};