#include <vector>
#include <string>

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

using std::string;

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

using glm::inverse;
using glm::transpose;
using glm::mat4;

using EntityManager::ComponentPtr;

RenderSystem::RenderSystem() : EntitySystem() { 
	this->family_name_ = "RenderSystem";

	std::string mesh = "Mesh";
	AddTypeByName(mesh);
	mesh_type_ = ComponentTypeManager::GetTypeFor(mesh);

	std::string transform = "Transform";
	AddTypeByName(transform);
	transform_type_ = ComponentTypeManager::GetTypeFor(transform);

	camera_type_ = ComponentTypeManager::GetTypeFor("Camera");

	camera_bits_ = 0;
	camera_bits_ |= camera_type_->bit();
	camera_bits_ |= transform_type_->bit();
}

RenderSystem::~RenderSystem() {
}

bool RenderSystem::CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity) {
	if ((camera_bits_ & entity->type_bits()) == camera_bits_) {
		shared_ptr<Component> component;

		component = EntityManager::GetComponent(entity, transform_type_);
		camera_transform_ = dynamic_pointer_cast<Transform>(component);

		component = EntityManager::GetComponent(entity, camera_type_);
		camera_ = dynamic_pointer_cast<Camera>(component);

		return false;
	}
	return true;
}

void RenderSystem::ProcessEntities(const EntityMap &entities) {
	EntityMap::const_iterator it, ite;
	ComponentPtr component;

	shared_ptr<Mesh> mesh;
	shared_ptr<Transform> transform;

	mat4 model, model_view, mvp, normal;
	mat4 view = camera_transform_->World();
	mat4 projection = camera_->Projection();
	
	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		component = EntityManager::GetComponent(it->second, mesh_type_);
		mesh = dynamic_pointer_cast<Mesh>(component);

		component = EntityManager::GetComponent(it->second, transform_type_);
		transform = dynamic_pointer_cast<Transform>(component);
		
		model = transform->World();
		model_view = view * model;
		mvp = projection * model_view;
		normal = inverse(transpose(model_view));

		mesh->material->PreRender();

		mesh->material->PushMatrices(model_view, projection, mvp, normal);
		mesh->geometry->Draw();

		mesh->material->PostRender();
	}
}

/*
void Renderer::Render() const {
	// get camera view matrix
	// get camera projection matrix (p)
	// get entity model matrix
	// calculate mv (view * model)
	// calculate mvp (p * mv)
	// calculate normal matrix (inverse(transpose(mv)))
	// attach + push material to gpu
	// push matrices to gpu
	// draw mesh
	// release material

	using glm::inverse;
	using glm::transpose;

	//mat4 view = camera_->entity_->transform_.World();
	mat4 projection = camera_->Projection();
	//mat4 model = entity_->transform_.World();
	//mat4 model_view = view * model;
	//mat4 mvp = projection * model_view;
	//mat4 normal = inverse(transpose(model_view));

	material_->PreRender();

	//material_->PushMatrices(model_view, projection, mvp, normal);
	mesh_->Draw();

	material_->PostRender();
}
*/