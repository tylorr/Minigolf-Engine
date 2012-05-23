#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "camera_controller.h"
#include "entity_manager.h"
#include "transform.h"

using glm::mat3;
using glm::vec3;

CameraController::CameraController() : EntitySystem() {
}

CameraController::~CameraController() {
}

void CameraController::Resolve() {
	camera_transform_ = EntityManager::GetComponent<Transform>(EntityManager::Find("Camera"), "Transform");
	ball_transform_ = EntityManager::GetComponent<Transform>(EntityManager::Find("Ball"), "Transform");
}

void CameraController::Process() {
	mat3 rotation = mat3(glm::mat4_cast(ball_transform_->rotation()));
	vec3 transformed = rotation * vec3(0, 3, 3);
	camera_transform_->set_position(ball_transform_->position() + vec3(transformed));
	camera_transform_->LookAt(*ball_transform_);
}

