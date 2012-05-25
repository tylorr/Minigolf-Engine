#include "ball_motor.h"
#include "entity.h"
#include "entity_manager.h"
#include "transform.h"
#include "time.h"
#include "input.h"

using boost::shared_ptr;

BallMotor::BallMotor() : EntitySystem("BallMotor") {
}

BallMotor::~BallMotor() {
}

void BallMotor::Resolve() {
	ball_ = EntityManager::Find("Ball");
}

void BallMotor::Process() {
	shared_ptr<Transform> ball_transform = EntityManager::GetComponent<Transform>(ball_, "Transform");

	float delta = Time::GetDeltaTime();

	// TODO: replace this later with proper force/acceleration input

	/*
	if (Input::GetKey("w")) {
		ball_transform->Translate(0, 0, -delta);
	}
	if (Input::GetKey("s")) {
		ball_transform->Translate(0, 0, delta);
	}
	if (Input::GetKey("a")) {
		ball_transform->Translate(-delta, 0, 0);
	}
	if (Input::GetKey("d")) {
		ball_transform->Translate(delta, 0, 0);
	}
	*/
}