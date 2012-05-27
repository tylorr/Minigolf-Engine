#ifndef BALL_MOTOR_H
#define BALL_MOTOR_H

#include <boost\shared_ptr.hpp>

#include "entity_system.h"

class Entity;

class BallMotor : public EntitySystem {
public:
	BallMotor();
	virtual ~BallMotor();

	virtual void Resolve();
	virtual void Process();

private:
	EntityPtr ball_;
};

#endif // BALL_MOTOR_H