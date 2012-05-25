#ifndef VOLUME_H
#define VOLUME_H

#include <vector>

#include "glm\glm.hpp"

#include "component.h"

struct Volume : public Component {

	Volume() : Component("Volume") { }

	std::vector<glm::vec3> vertices;
	glm::vec3 normal;
};

#endif // VOLUME_H