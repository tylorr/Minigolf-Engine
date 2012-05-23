#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "third_person_camera_system.h"
#include "entity_manager.h"
#include "transform.h"

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using glm::quat;
using glm::mat4;
using glm::vec3;

ThirdPersonCameraSystem::ThirdPersonCameraSystem() {
}

ThirdPersonCameraSystem::~ThirdPersonCameraSystem() {
}

void ThirdPersonCameraSystem::Process() {
	/*
	shared_ptr<Entity> camera = EntityManager::Find("Camera");
	shared_ptr<Transform> camera_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(camera, "Transform"));

	shared_ptr<Entity> ball = EntityManager::Find("Ball");
	shared_ptr<Transform> ball_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(ball, "Transform"));
	*/
	//camera_transform->position = -ball_transform->position + vec3(0, -1.0f, -5.0f);

	//mat4 mat = glm::lookAt(camera_transform->position, -ball_transform->position, vec3(0, 1, 0));

	//camera_transform->rotation = glm::quat_cast(mat);
}