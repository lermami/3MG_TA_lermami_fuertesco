#pragma once
#include<unordered_map>
#include<memory>
#include<optional>
#include<cassert>
#include <utility>

#include "buffer.hpp"
#include "vector_2.hpp"
#include "vector_3.hpp"
#include "vector_4.hpp"
#include "matrix_4.hpp"
#include "sound/soundsource.h"
#include "light.hpp"
#include "Window.hpp"
#include "Engine.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex {
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec4 color;

	bool operator==(const Vertex& o) const{
		return pos == o.pos && normal == o.normal && uv == o.uv;
	}
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

struct Geometry {
	std::vector<Vertex> vertex_;
	std::vector<unsigned> indices_;
};

struct RenderComponent {
	Geometry geometry_;
	std::shared_ptr<Buffer> elements_buffer_;
	std::shared_ptr<Buffer> order_buffer_;
	unsigned int program_ = -1;
	unsigned int texture_ = -1;
};

struct CameraComponent {
	Vec3 pos_;
	Vec3 forward_;
	Vec3 up_;
	Vec3 right_;

	float speed_;
	float sensitivity_;

	ProjectionMode projectionMode_;

	CameraComponent(){
		pos_ = Vec3(0.0f, 0.0f, 0.0f);
		right_ = Vec3(0.0f, 0.0f, 0.0f);
		up_ = Vec3(0.0f, 1.0f, 0.0f);
		forward_ = Vec3(0.0f, 0.0f, -1.0f);

		speed_ = 1.0f;
		sensitivity_ = 1.0f;

		projectionMode_ = ProjectionMode::kPerspective;
	}

	void setProjectionMode(ProjectionMode mode) {
		projectionMode_ = mode;
	}
	ProjectionMode getProjectionMode() {
		return projectionMode_;
	}

	glm::mat4 getPerspectiveMatrix(float fov, float aspect, float near, float far){
		return glm::perspective(glm::radians(fov), aspect, near, far);
	}
	glm::mat4 getOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far) {
		return glm::ortho(left, right, bottom, top, near, far);
	}
	glm::mat4 getViewMatrix(Vec3 target, Vec3 up) {
		glm::vec3 c = { pos_.x, pos_.y, pos_.z };
		glm::vec3 t = { target.x, target.y, target.z };
		glm::vec3 u = { up.x, up.y, up.z };

		return glm::lookAt(c, t, u);
	}

	void doRender(Window* w) {
		for (int i = 0; i < w->getProgramListSize(); i++) {
			unsigned program = w->getProgram(i);
			glUseProgram(program);

			switch (projectionMode_) {
			case ProjectionMode::kPerspective:
				glm::mat4 perspective = getPerspectiveMatrix(60.0f, 1024.0f / 768.0f, 0.01f, 100000.0f);

				glUniformMatrix4fv(glGetUniformLocation(program, "u_p_matrix"), 1, GL_FALSE, glm::value_ptr(perspective));
				break;
			case ProjectionMode::kOrthogonal:
				glm::mat4 ortographic = getOrthogonalMatrix(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100000.0f);

				glUniformMatrix4fv(glGetUniformLocation(program, "u_o_matrix"), 1, GL_FALSE, glm::value_ptr(ortographic));
				break;
			}

			//View
			glm::mat4 view = getViewMatrix(pos_ + forward_, up_);
			GLint viewMatrixLoc = glGetUniformLocation(program, "u_v_matrix");
			glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

			//Camera position
			GLint camPosLoc = glGetUniformLocation(program, "u_camera_pos");
			glUniform1fv(camPosLoc, sizeof(float) * 3, &pos_.x);
		}
	}
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

	ComponentManager(Engine& e) {
		e.componentM_ = this;

		add_component_class<RenderComponent>();
		add_component_class<TransformComponent>();
		add_component_class<AudioComponent>();
		add_component_class<LightComponent>();
		add_component_class<CameraComponent>();
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