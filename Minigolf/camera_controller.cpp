#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "camera_controller.h"
#include "entity_manager.h"
#include "transform.h"
#include "input.h"
#include "time.h"

using glm::mat3;
using glm::vec3;
using boost::shared_ptr;

CameraController::CameraController() : EntitySystem("CameraController") {
}

CameraController::~CameraController() {
}

void CameraController::Init() {
	third_person_.reference = vec3(0, 3.0f, 3.0f);
	third_person_.up = vec3(0, 1, 0);

	top_down_.reference = vec3(0, 3.0f, 0);
	top_down_.up = vec3(0, 0, -1);

	profile_ = third_person_;
	free_ = false;

	head_degrees_ = 0;
	pitch_degrees_ = 0;
}

void CameraController::Resolve() {
	camera_ = EntityManager::Find("Camera");
	ball_ = EntityManager::Find("Ball");
}

void CameraController::Process() {
	if (Input::GetKeyDown("1")) {
		free_ = false;
		profile_ = third_person_;
	} else if (Input::GetKeyDown("2")) {
		free_ = false;
		profile_ = top_down_;
	} else if (Input::GetKeyDown("3")) {
		free_ = true;
	}

	TransformPtr ball_transform = EntityManager::GetComponent<Transform>(ball_, "Transform");
	TransformPtr camera_transform = EntityManager::GetComponent<Transform>(camera_, "Transform");

	// are we in free mode?
	if (free_) {
		// check for input to control free camera

		float delta = Time::GetDeltaTime();
		float speed = 2.0f;

		vec3 camera_forward = camera_transform->forward();
		//camera_forward.z *= -1;
		vec3 velocity(0);

		if (Input::GetKey("w")) {
			velocity -= camera_forward * speed;
		}
		if (Input::GetKey("a")) {
			velocity += camera_transform->right() * -speed;
		}
		if (Input::GetKey("s")) {
			velocity -= camera_forward * -speed;
		}
		if (Input::GetKey("d")) {
			velocity += camera_transform->right() * speed;
		}

		camera_transform->Translate(velocity * delta);

		float rot_speed = 60.0f;
		head_degrees_ = 0;
		pitch_degrees_ = 0;

		if (Input::GetKey("i")) {
			pitch_degrees_ -= rot_speed * delta;
		}
		if (Input::GetKey("j")) {
			head_degrees_ += rot_speed * delta;
		}
		if (Input::GetKey("k")) {
			pitch_degrees_ += rot_speed * delta;
		}
		if (Input::GetKey("l")) {
			head_degrees_ -= rot_speed * delta;
		}

		// TODO: this needs to be fixed, the camera will not rotate about its 
		// local x-axis
		quat q;
		if (head_degrees_ != 0) {
			q = glm::rotate(quat(), head_degrees_, vec3(0, 1, 0));
			camera_transform->set_rotation(q * camera_transform->rotation());
		}
		
		if (pitch_degrees_ != 0) {
			q = glm::rotate(quat(), pitch_degrees_, vec3(1, 0, 0));
			camera_transform->set_rotation(camera_transform->rotation() * q);
		}
		
		
	} else {
		// use a camera profile
		mat3 rotation = mat3(glm::mat4_cast(ball_transform->rotation()));
		vec3 transformed = rotation * profile_.reference;
		camera_transform->set_position(ball_transform->position() + vec3(transformed));
		camera_transform->LookAt(*ball_transform, rotation * profile_.up);
	}
}

                                                                               