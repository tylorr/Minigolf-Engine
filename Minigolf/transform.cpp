#include "transform.h"

using glm::translate;
using glm::rotate;
using glm::scale;

Transform::Transform() {
	dirty_ = true;

	position_ = vec3(0.0f);
	rotation_ = quat();
	scale_ = vec3(1.0f);

	parent_ = NULL;
}

Transform::~Transform() {
}

mat4 Transform::Local() const {
	using glm::mat4_cast;

	mat4 result;

	// combine translation, rotation, and scale into a matrix
	result = translate(mat4(1.0f), position_);
	result *= mat4_cast(rotation_);
	result = scale(result, scale_);

	return result;
}

mat4 Transform::World() {
	mat4 local;
	mat4 parent_world;

	// If this is root of tree then world transform equals local transform
	if (parent_ != NULL) {

		parent_world = parent_->World();

		// If the local matrix or the parents world matrix have changed
		// then update our world matrix
		if (dirty_ || parent_world != cached_parent_world_) {
			cached_parent_world_ = parent_world;
			
			local = Local();
			world_ = parent_world * local;
			dirty_ = false;
		}

		return world_;
	} else {
		return Local();
	}
}