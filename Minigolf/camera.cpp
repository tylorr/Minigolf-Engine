#include "glm\gtc\matrix_transform.hpp"

#include "camera.h"

Camera::Camera() {
	orthographic_ = false;
	orthographic_size_ = 1.0f;
	aspect_ratio_ = 1.5f;
	field_of_view_ = 60.0f;
	near_plane_ = 1.0f;
	far_plane_ = 1000.0f;
}

Camera::~Camera() {
}

mat4 Camera::Projection() const {
	// TODO: Figure out a way to cache this

	using glm::ortho;
	using glm::perspective;

	mat4 projection;

	// create orthographic or perspective projection based on orthographic_ boolean
	if (orthographic_) {
		projection = ortho(-orthographic_size_ * aspect_ratio_, orthographic_size_ * aspect_ratio_, orthographic_size_, -orthographic_size_, near_plane_, far_plane_);
	} else {
		projection = perspective(field_of_view_, aspect_ratio_, near_plane_, far_plane_);
	}

	return projection;
}