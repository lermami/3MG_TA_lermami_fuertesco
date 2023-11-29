#include "default_systems.hpp"
#include <GL/glew.h>

void init_vertex_system(RenderComponent& render, std::vector<Vertex>& v,
	std::vector<unsigned>& indices_, unsigned int program) {

	for (int i = 0; i < v.size(); i++) {
		render.vertex_.push_back(v[i]);
	}

	render.elements_buffer_ = std::make_shared<Buffer>();
	render.elements_buffer_.get()->init((unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));
	render.elements_buffer_.get()->uploadData(&render.vertex_[0], (unsigned)(sizeof(render.vertex_[0]) * render.vertex_.size()));

	render.order_buffer_ = std::make_shared<Buffer>();
	render.order_buffer_.get()->init(indices_.size());
	render.order_buffer_.get()->uploadData(&indices_[0], (unsigned)(indices_.size() * sizeof(unsigned)));

	/*
	glGenVertexArrays(1, &render.VAO_);
	glBindVertexArray(render.VAO_);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render.vertex_[0]), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	render.program_ = program;
}

void init_transform_system(TransformComponent& transform, Vec3& pos, Vec3& rot, Vec3& size) {
	transform.pos_ = pos;
	transform.size_ = size;
	transform.rot_ = rot;
}

void init_color_system(RenderComponent& render, float r, float g, float b, float a) {
	for (auto& v : render.vertex_) {
		v.r_ = r;
		v.g_ = g;
		v.b_ = b;
		v.a_ = a;
	}

}

void move_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 mov) {

	auto r = transforms.begin();

	for (; r != transforms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.pos_ += mov;
	}

}

void rotate_system(std::vector<std::optional<TransformComponent>>& transforms, Vec3 rot) {
	auto r = transforms.begin();

	for (; r != transforms.end(); r++) {
		if (!r->has_value()) continue;
		auto& transfrom = r->value();
		transfrom.rot_ += rot;
	}
}

size_t on_click_system(std::vector<std::optional<TransformComponent>>& transforms, float mouse_x, float mouse_y) {

	mouse_x = (mouse_x / 1024 * 2) - 1;
	mouse_y = ((mouse_y / 768 * 2) - 1) * -1;

	auto r = transforms.begin();
	size_t e = 1;

	for (; r != transforms.end(); r++, e++) {
		if (!r->has_value()) continue;

		//TODO: Add size
		float x1 = r->value().pos_.x - r->value().size_.x;
		float x2 = r->value().pos_.x + r->value().size_.x;
		float y1 = r->value().pos_.y - r->value().size_.y;
		float y2 = r->value().pos_.y + r->value().size_.y;

		if (mouse_x > x1 && mouse_x < x2 &&
			mouse_y > y1 && mouse_y < y2)
			return e;

	}

	return 0;
}

void set_position_system(TransformComponent& transform, Vec3 pos) {
	pos.x = (pos.x / 1024 * 2) - 1;
	pos.y = ((pos.y / 768 * 2) - 1) * -1;

	transform.pos_ = pos;
}

void render_system(std::vector<std::optional<RenderComponent>>& renders, std::vector<std::optional<TransformComponent>>& transforms) {

	auto r = renders.begin();
	auto t = transforms.begin();

	for (; r != renders.end(); r++, t++) {
		if (!r->has_value() && !t->has_value()) continue;
		auto& render = r->value();
		auto& transform = t->value();

		Mat4& m = transform.model_matrix_;

		m = m.Identity();
		m = m.Multiply(m.Translate(transform.pos_));
		m = m.Multiply(m.RotateX(transform.rot_.x).Multiply(m.RotateY(transform.rot_.y).Multiply(m.RotateZ(transform.rot_.z))));
		m = m.Multiply(m.Scale(transform.size_));
		m = m.Transpose();

		glUseProgram(render.program_);
		GLint modelMatrixLoc = glGetUniformLocation(render.program_, "u_m_matrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m.m[0]);

		render.elements_buffer_.get()->bind(kTarget_VertexData);
		render.elements_buffer_.get()->uploadFloatAttribute(0, 3, sizeof(render.vertex_[0]), (void*)0);
		render.elements_buffer_.get()->uploadFloatAttribute(1, 4, sizeof(render.vertex_[0]), (void*)(3 * sizeof(float)));

		auto order_buffer = render.order_buffer_.get();
		order_buffer->bind(kTarget_Elements);
		glDrawElements(GL_TRIANGLES, order_buffer->size(), GL_UNSIGNED_INT, 0);
	}
}