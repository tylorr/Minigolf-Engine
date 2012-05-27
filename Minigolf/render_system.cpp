#include <vector>
#include <string>
#include <typeinfo.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\string_cast.hpp"

#include "render_system.h"
#include "component_type.h"
#include "component_type_manager.h"
#include "entity_manager.h"
#include "component.h"
#include "mesh.h"
#include "transform.h"
#include "camera.h"
#include "entity.h"
#include "basic_material.h"

using std::string;

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

using glm::inverse;
using glm::transpose;
using glm::mat4;
using glm::mat3;

RenderSystem::RenderSystem() {

	std::string mesh = "Mesh";
	TrackType<Mesh>();
	mesh_type_ = ComponentTypeManager::GetTypeFor<Mesh>();

	std::string transform = "Transform";
	TrackType<Transform>();
	transform_type_ = ComponentTypeManager::GetTypeFor<Transform>();
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::ProcessEntities(const EntityMap &entities) {
	// todo: check for existance of camera
	// todo: create component entity mapper like artemis has

	if (entities.empty()) {
		return;
	}

	EntityMap::const_iterator it, ite;
	ComponentPtr component;

	MeshPtr mesh;
	TransformPtr transform;

	// todo: make initialize step so this doesn't have to happen every time
	EntityPtr camera = EntityManager::Find("Camera");
	CameraPtr camera_comp = EntityManager::GetComponent<Camera>(camera);
	TransformPtr camera_transform = EntityManager::GetComponent<Transform>(camera);

	mat4 model, model_view, mvp;
	mat3 normal;
	mat4 view;

	// view matrix is R^-1*-T
	quat rot = glm::conjugate(camera_transform->rotation());
	view = glm::mat4_cast(rot);
	view = glm::translate(view, -camera_transform->position());

	mat4 projection = camera_comp->Projection();
	
	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		mesh = EntityManager::GetComponent<Mesh>(it->second);

		transform = EntityManager::GetComponent<Transform>(it->second);

		model = transform->world();
		model_view = view * model;
		mvp = projection * model_view;
		normal = inverse(transpose(mat3(model_view))); // note: always remember that normal_mat needs to be mat3

		shared_ptr<BasicMaterial> material = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);

		vec4 light_pos = material->light_position_;

		// hack to have light move with world
		// todo: implement light as entity
		material->light_position_ = model_view * light_pos;

		// do things like setup colors and lights
		// and attach shader program
		mesh->material->PreRender();

		material->light_position_ = light_pos;

		// push matrices up
		mesh->material->PushMatrices(model_view, projection, mvp, normal);

		// call draw
		mesh->geometry->Draw();

		// let go of shader program
		mesh->material->PostRender();
	}
}