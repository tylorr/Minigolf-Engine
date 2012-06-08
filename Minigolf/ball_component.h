#ifndef BALL_COMPONENT_H
#define BALL_COMPONENT_H

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"

struct BallComponent : public Component {
	EntityPtr current_tile;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	BallComponent() : velocity(0), acceleration(0) { }
};

typedef boost::shared_ptr<BallComponent> BallComponentPtr;

#endif // BALL_COMPONENT_H