#ifndef CAMERA_H
#define CAMERA_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "component.h"

struct Camera: public Component {
	bool orthographic;
	float orthographic_size; // this represents half the height of the orthographic projection
	float aspect_ratio;
	float field_of_view;
	float near_plane;
	float far_plane;

	Camera() : Component("Camera") { }

	glm::mat4 Projection() const;
};

typedef boost::shared_ptr<Camera> CameraPtr;

#endif // CAMERA_H