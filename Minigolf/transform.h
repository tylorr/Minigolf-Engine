#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

#include "component.h"

using glm::vec3;
using glm::quat;
using glm::mat4;

struct Transform : public Component {
public:
	vec3 position;
	vec3 scale;
	quat rotation;

	Transform() : Component("Transform") { }
};

#endif // TRANSFORM_H