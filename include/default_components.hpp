#pragma once

#include "buffer.hpp"
#include "vector_2.hpp"
#include "vector_3.hpp"
#include "vector_4.hpp"
#include "matrix_4.hpp"
#include "sound/soundsource.h"
#include "light.hpp"
#include "Window.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
		std::optional<T> a;
		components_.emplace_back(a);
	}

	virtual void add_component(int position) override {
		T a;
		//-1 because position refers to an entity that starts in 1
		components_[(size_t)position - 1] = a;
	}

	void add_component_at(int position, T& component) {
		assert(components_[(size_t)position - 1] == std::nullopt);

		components_[(size_t)position - 1] = component;

	}

	virtual size_t size() override {
		return components_.size();
	}

	virtual void delete_component(size_t id) override {
		assert(id != 0);
		components_[id - 1].reset();
		components_[id - 1] = std::nullopt;
	}
};

struct Vertex {
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec4 color;

	bool operator==(const Vertex& o) const {
		return pos == o.pos && normal == o.normal && uv == o.uv;
	}
};

struct TransformComponent {
	Mat4 model_matrix_;

	Vec3 pos_;
	Vec3 rot_;
	Vec3 size_;

	TransformComponent() = default;

	TransformComponent(Vec3 pos, Vec3 rot = Vec3(0.0f, 0.0f, 0.0f), Vec3 size = Vec3(1.0f, 1.0f, 1.0f)) {
		pos_ = pos;
		rot_ = rot;
		size_ = size;
	}
};

struct ColorComponent {
	Vec4 color_;

	ColorComponent() = default;
	ColorComponent(Vec4 color) {
		color_ = color;
	}
};

struct AudioComponent {
	AudioComponent(SoundBuffer& buff, std::string name, ALfloat pos[3],
		ALfloat speed[3], float gain = 1.0f, float pitch = 1.0f, bool play = false)
		: sound_source_(name, pos, speed, gain, pitch) {

		sound_source_.addSound(&buff);
		if (play)
			sound_source_.Play();
	}

	AudioComponent() = default;
	AudioComponent& operator=(const AudioComponent& o) = default;

	SoundSource sound_source_;
};

struct BoxColliderComponent {
	Vec3 extent_;
	Vec3 center_offset_;

	BoxColliderComponent() = default;
	BoxColliderComponent(Vec3 extent, Vec3 center_offset = Vec3(0.0f, 0.0f, 0.0f)) {
		extent_ = extent;
		center_offset_ = center_offset;
	}
};

struct Geometry {
	std::vector<Vertex> vertex_;
	std::vector<unsigned> indices_;
};

struct RenderComponent {
	std::string name_;
	std::string elements_buffer_;
	std::string order_buffer_;
	unsigned int program_ = -1;
	unsigned int texture_ = -1;

	RenderComponent() = default;

	RenderComponent(const char* name, const char* elements, const char* order, unsigned program, unsigned texture) {
		name_ = name;
		elements_buffer_ = elements;
		order_buffer_ = order;
		program_ = program;
		texture_ = texture;
	}
};

struct CameraComponent {
	std::string name_;

	Vec3 pos_;

	Vec3 forward_;
	Vec3 up_;
	Vec3 right_;

	float speed_;
	float sensitivity_;

	ProjectionMode projectionMode_;

	float last_alpha;
	float last_omega;

	CameraComponent() {
		right_ = Vec3(0.0f, 0.0f, 0.0f);
		up_ = Vec3(0.0f, 1.0f, 0.0f);
		forward_ = Vec3(0.0f, 0.0f, -1.0f);

		speed_ = 1.0f;
		sensitivity_ = 1.0f;

		projectionMode_ = ProjectionMode::kPerspective;

		last_alpha = -1.57f;
		last_omega = 0.0f;
	};

	CameraComponent(std::string name, float speed, float sensitivity, ProjectionMode mode = ProjectionMode::kPerspective) {
		name_ = name;

		right_ = Vec3(0.0f, 0.0f, 0.0f);
		up_ = Vec3(0.0f, 1.0f, 0.0f);
		forward_ = Vec3(0.0f, 0.0f, -1.0f);

		speed_ = speed;
		sensitivity_ = sensitivity;

		projectionMode_ = mode;

		last_alpha = -1.57f;
		last_omega = 0.0f;
	}
};
