#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "camera_controller.h"
#include "entity_manager.h"
#include "transform.h"

using glm::mat3;
using glm::vec3;
using boost::shared_ptr;

CameraController::CameraController() : EntitySystem("CameraController") {
}

CameraController::~CameraController() {
}

void CameraController::Resolve() {
	camera_ = EntityManager::Find("Camera");
	ball_ = EntityManager::Find("Ball");
}

void CameraController::Process() {
	shared_ptr<Transform> ball_transform = EntityManager::GetComponent<Transform>(ball_, "Transform");
	shared_ptr<Transform> camera_transform = EntityManager::GetComponent<Transform>(camera_, "Transform");

	mat3 rotation = mat3(glm::mat4_cast(ball_transform->rotation()));
	vec3 transformed = rotation * vec3(0, 3, 3);
	camera_transform->set_position(ball_transform->position() + vec3(transformed));
	camera_transform->LookAt(*ball_transform);
}

