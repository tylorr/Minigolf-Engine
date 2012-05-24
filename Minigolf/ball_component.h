#ifndef BALL_COMPONENT_H
#define BALL_COMPONENT_H

#include "component.h"
#include "entity.h"

struct BallComponent : public Component {
	boost::shared_ptr<Entity> current_tile;

	BallComponent() : Component("BallComponent") { }
};

#endif // BALL_COMPONENT_H