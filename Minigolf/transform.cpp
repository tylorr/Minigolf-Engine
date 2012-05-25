#include "glm\gtc\matrix_transform.hpp"

#include "transform.h"

void Transform::set_position(const vec3 &position) {
	position_ = position;
	UpdateMatrix();
}

void Transform::set_scale(const vec3 &scale) {
	scale_ = scale;
	UpdateMatrix();
}

void Transform::set_rotation(const quat &rotation) {
	rotation_ = rotation;
	UpdateMatrix();
}


void Transform::UpdateMatrix()  {
	// combine translation, rotation, and scale into a matrix
	world_ = glm::translate(mat4(1.0f), position_);
	world_ *= glm::mat4_cast(rotation_);

	UpdateAxes(); // figure out if this should go before or after the scaling

	world_ = glm::scale(world_, scale_);
}

void Transform::UpdateAxes() {
	using glm::mat3;

	mat3 temp = mat3(world_);
	right_ = glm::normalize(temp[0]);
	up_ = glm::normalize(temp[1]);
	forward_ = glm::normalize(temp[2]);
}

void Transform::Translate(const vec3 &translation) {
	Translate(translation.x, translation.y, translation.z);
}

void Transform::Translate(const float &x, const float &y, const float &z) {
	position_.x += x;
	position_.y += y;
	position_.z += z;

	UpdateMatrix();
}

void Transform::LookAt(const Transform &target, const vec3 &up) {
	LookAt(target.position_, up);
}

void Transform::LookAt(const vec3 &target, const vec3 &up) {
	quat q = glm::quat_cast(glm::lookAt(position_, target, up));
	rotation_ = glm::conjugate(q);

	UpdateMatrix();
}

void Transform::Rotate(const vec3 &euler_angles) {
	Rotate(vec3(0, 0, 1), euler_angles.z);
	Rotate(vec3(1, 0, 0), euler_angles.x);
	Rotate(vec3(0, 1, 0), euler_angles.y);
}

void Transform::Rotate(const float &x, const float &y, const float &z) {
	Rotate(vec3(x, y, z));
}

void Transform::Rotate(const vec3 &axis, const float &angle) {
	rotation_ = glm::rotate(rotation_, angle, axis);

	UpdateMatrix();
}


/*
mat4 Transform::World() {
	mat4 local;
	mat4 parent_world;

	// If this is root of tree then world transform equals local transform
	if (parent != shared_ptr<Transform>()) {

		parent_world = parent->World();

		// If the local matrix or the parents world matrix have changed
		// then update our world matrix
		if (dirty_ || parent_world != cached_parent_world_) {
			cached_parent_world_ = parent_world;
			
			local = Local();
			world_ = parent_world * local;
			//dirty_ = false;
		}

		return world_;
	} else {
		return Local();
	}
}
*/