#include "glm\gtc\matrix_transform.hpp"

#include "transform.h"

using boost::shared_ptr;

mat4 Transform::Local() const {
	using glm::mat4_cast;

	mat4 result;

	// combine translation, rotation, and scale into a matrix
	result = glm::translate(mat4(1.0f), position);
	result *= mat4_cast(rotation);
	result = glm::scale(result, scale);

	return result;
}

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
			dirty_ = false;
		}

		return world_;
	} else {
		return Local();
	}
}