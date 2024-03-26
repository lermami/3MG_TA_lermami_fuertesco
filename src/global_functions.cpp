#include "component_manager.hpp"

void change_entity_color(ColorComponent* color, float r, float g, float b, float a) {
	if (color) {
		color->color_.x = r;
		color->color_.y = g;
		color->color_.z = b;
		color->color_.w = a;
	}
}

void set_entity_transform(TransformComponent* transform, Vec3 position, Vec3 rotation, Vec3 size) {
	if (transform) {
		transform->pos_ = position;
		transform->rot_ = rotation;
		transform->size_ = size;
	}
}

void set_entity_position(TransformComponent* transform, Vec3 pos) {
	if (transform) {
		transform->pos_ = pos;
	}
}

bool entities_are_colliding(ComponentManager& componentM, size_t entity1, size_t entity2) {

	//Colliders
	auto collider1 = componentM.get_component<BoxColliderComponent>(entity1);
	auto collider2 = componentM.get_component<BoxColliderComponent>(entity2);

	//Transform
	auto transform1 = componentM.get_component<TransformComponent>(entity1);
	auto transform2 = componentM.get_component<TransformComponent>(entity2);

	if (collider1 == nullptr || collider2 == nullptr ||
		 transform1 == nullptr || transform2 == nullptr){
		printf("ERROR. Entities not valid.");
		return false;
	};

	Vec3 obj1 = transform1->pos_ + collider1->center_offset_;
	Vec3 obj2 = transform2->pos_ + collider2->center_offset_;


	// check the X axis
	if (glm::abs(obj1.x - obj2.x) < collider1->extent_.x + collider2->extent_.x) {
		// check the Y axis
		if (glm::abs(obj1.y - obj2.y) < collider1->extent_.y + collider2->extent_.y) {
			// check the Z axis
			if (glm::abs(obj1.z - obj2.z) < collider1->extent_.z + collider2->extent_.z) {
				return true;
			}
		}
	}

	return false;
}
