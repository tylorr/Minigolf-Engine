#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "camera_controller.h"
#include "entity_manager.h"
#include "transform.h"
#include "input.h"

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

	shared_ptr<Transform> ball_transform = EntityManager::GetComponent<Transform>(ball_, "Transform");
	shared_ptr<Transform> camera_transform = EntityManager::GetComponent<Transform>(camera_, "Transform");

	if (free_) {
	} else {
		mat3 rotation = mat3(glm::mat4_cast(ball_transform->rotation()));
		vec3 transformed = rotation * profile_.reference;
		camera_transform->set_position(ball_transform->position() + vec3(transformed));
		camera_transform->LookAt(*ball_transform, rotation * profile_.up);
	}
}

