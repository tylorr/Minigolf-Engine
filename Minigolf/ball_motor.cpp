#include "ball_motor.h"
#include "entity.h"
#include "entity_manager.h"
#include "transform.h"
#include "time.h"
#include "input.h"
#include "ball_component.h"

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
	shared_ptr<BallComponent> ball_comp = EntityManager::GetComponent<BallComponent>(ball_, "BallComponent");

	float delta = Time::GetDeltaTime();
	float speed = -10.0f;
	float rot_speed = 50.0f;

	// TODO: replace this later with proper force/acceleration input
	//vec3 velocity;
	
	ball_comp->acceleration = vec3();

	/*
	if (Input::GetKey("up")) {
		ball_comp->acceleration += vec3(0, 0, speed);
	}
	if (Input::GetKey("down")) {
		ball_comp->acceleration += vec3(0, 0, -speed);
	}
	if (Input::GetKey("left")) {
		ball_comp->acceleration += vec3(speed, 0, 0);
	}
	if (Input::GetKey("right")) {
		ball_comp->acceleration += vec3(-speed, 0, 0);
	}
	*/

	static const float epsilon = 0.01f;

	if (glm::length(ball_comp->velocity) < epsilon) {
	
		if (Input::GetKey("left")) {
			ball_transform->Rotate(vec3(0, 1, 0), rot_speed * delta);
		}
		if (Input::GetKey("right")) {
			ball_transform->Rotate(vec3(0, 1, 0), -rot_speed * delta);
		}

		if (Input::GetKeyUp("t")) {
			ball_comp->velocity += ball_transform->forward() * speed;
		}
	
	}
	
}