#include <vector>
#include <string>

#include <boost\shared_ptr.hpp>

#include "render_system.h"
#include "component_type.h"
#include "component_type_manager.h"
#include "entity_manager.h"
#include "component.h"

using boost::shared_ptr;
using std::string;


RenderSystem::RenderSystem() : EntitySystem() { 
	this->family_name_ = "RenderSystem";

	AddTypeByName("RenderComponent");
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::ProcessEntities(const EntityMap &entities) {
	using EntityManager::ComponentPtr;

	EntityMap::const_iterator it, ite;
	shared_ptr<ComponentType> renderType = ComponentTypeManager::GetTypeFor("RenderComponent");
	ComponentPtr component;
	
	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		component = EntityManager::GetComponent(it->second, renderType);
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