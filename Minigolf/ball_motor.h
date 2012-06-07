#ifndef BALL_MOTOR_H
#define BALL_MOTOR_H

#include <boost\shared_ptr.hpp>

#include "entity_system.h"

class Entity;

class BallMotor : public EntitySystem {
public:
	BallMotor(const int &layer);

	virtual void Process();
};

#endif // BALL_MOTOR_H