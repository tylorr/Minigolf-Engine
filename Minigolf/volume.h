#ifndef VOLUME_H
#define VOLUME_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "component.h"

struct Volume : public Component {
	std::vector<glm::vec3> vertices;
	glm::vec3 normal;
};

typedef boost::shared_ptr<Volume> VolumePtr;

#endif // VOLUME_H