#include <vector>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

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

RenderSystem::RenderSystem(const int &layer) : EntitySystem(layer) {
	TrackType<Transform>();
	TrackType<Mesh>();
}

void RenderSystem::Init() {
	camera_ = EntityManager::Find("Camera");
}

void RenderSystem::ProcessEntities(const EntityMap &entities) {

	if (entities.empty()) {
		printf("Warning: RenderSystem: Entity list is empty\n");
		return;
	}

	

	EntityMap::const_iterator it, ite;
	ComponentPtr component;

	MeshPtr mesh;
	TransformPtr transform;

	// todo: make initialize step so this doesn't have to happen every time
	
	CameraPtr camera_comp = camera_mapper_(camera_);
	TransformPtr camera_transform = transform_mapper_(camera_);

	mat4 model, view, projection, model_view, mvp;
	mat3 normal;

	// view matrix is R^-1*-T
	quat rot = glm::conjugate(camera_transform->rotation());
	view = glm::mat4_cast(rot);
	view = glm::translate(view, -camera_transform->position());

	projection = camera_comp->Projection();
	
	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		mesh = mesh_mapper_(it->second);

		transform = transform_mapper_(it->second);

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