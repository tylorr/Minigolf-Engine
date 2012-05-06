#ifndef CAMERA_H
#define CAMERA_H

#include "glm\glm.hpp"

#include "component.h"

using glm::mat4;

class Camera : public Component {
public:
	bool orthographic_;
	float orthographic_size_; // this represents half the height of the orthographic projection
	float aspect_ratio_;
	float field_of_view_;
	float near_plane_;
	float far_plane_;

	Camera();
	~Camera();

	mat4 Projection();
};

#endif // CAMERA_H