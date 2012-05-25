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
	float speed = 1.5f;

	// TODO: replace this later with proper force/acceleration input
	vec3 velocity;
	
	if (Input::GetKey("up")) {
		velocity += vec3(0, 0, -speed);
	}
	if (Input::GetKey("down")) {
		velocity += vec3(0, 0, speed);
	}
	if (Input::GetKey("left")) {
		velocity += vec3(-speed, 0, 0);
	}
	if (Input::GetKey("right")) {
		velocity += vec3(speed, 0, 0);
	}

	ball_transform->Translate(velocity * delta);

	if (glm::length(velocity) > 0) {
		ball_transform->LookAt(ball_transform->position() + velocity);
	}
	
}