#ifndef BALL_MOTOR_H
#define BALL_MOTOR_H

#include <boost\shared_ptr.hpp>
#include <vector>
#include "entity_system.h"
#include "component_mapper.h"


class Entity;
struct GuiText;

class BallMotor : public EntitySystem {
public:
	float max_speed_;
	float speed_step_;

	BallMotor(const int &layer);
	BallMotor(const int &layer, const std::string &script);

	virtual void Init();
	void ReloadScript();
	virtual void Process();
private:
	ComponentMapper<GuiText> gui_text_mapper_;

	
};

#endif // BALL_MOTOR_H