#include "glm\gtc\matrix_transform.hpp"

#include "camera.h"

using glm::mat4;

mat4 Camera::Projection() const {
	mat4 projection;

	if (orthographic) {
		projection = glm::ortho(-orthographic_size * aspect_ratio, orthographic_size * aspect_ratio, orthographic_size, -orthographic_size, near_plane, far_plane);
	} else {
		projection = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
	}

	return projection;
}