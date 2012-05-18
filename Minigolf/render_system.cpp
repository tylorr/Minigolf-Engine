#include <vector>
#include <string>

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

using EntityManager::ComponentPtr;

RenderSystem::RenderSystem(const bool &relative, const glm::vec3 &reference, const glm::vec3 &up) : EntitySystem() { 
	relative_ = relative;
	reference_ = reference;
	up_ = up;

	this->family_name_ = "RenderSystem";

	std::string mesh = "Mesh";
	AddTypeByName(mesh);
	mesh_type_ = ComponentTypeManager::GetTypeFor(mesh);

	std::string transform = "Transform";
	AddTypeByName(transform);
	transform_type_ = ComponentTypeManager::GetTypeFor(transform);
}

RenderSystem::~RenderSystem() {
}

/*
bool RenderSystem::CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity) {

	// I think this is the right way about getting one and only one camera object
	// if you think there is another way let me know
	// todo: create some sort of camera selector component to allow for multiple cameras
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
*/

void RenderSystem::ProcessEntities(const EntityMap &entities) {
	// todo: check for existance of camera
	// todo: create component entity mapper like artemis has

	if (entities.empty()) {
		return;
	}

	EntityMap::const_iterator it, ite;
	ComponentPtr component;

	shared_ptr<Mesh> mesh;
	shared_ptr<Transform> transform;

	// todo: make initialize step so this doesn't have to happen every time
	shared_ptr<Entity> camera = EntityManager::Find("Camera");
	shared_ptr<Camera> camera_comp = boost::dynamic_pointer_cast<Camera>(EntityManager::GetComponent(camera, "Camera"));
	shared_ptr<Transform> camera_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(camera, "Transform"));

	shared_ptr<Entity> root = EntityManager::Find("Root");
	shared_ptr<Transform> root_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(root, "Transform"));

	shared_ptr<Entity> ball = EntityManager::Find("Ball");
	shared_ptr<Transform> ball_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(ball, "Transform"));

	mat4 model, model_view, mvp;
	mat3 normal;
	mat4 view;

	if (relative_) {
		mat3 rotation = mat3(glm::mat4_cast(ball_transform->rotation));
		vec3 transformed = rotation * reference_;
		vec3 camera_pos = ball_transform->position + vec3(transformed);
		view = glm::lookAt(camera_pos, ball_transform->position, up_);
	} else {
		view = glm::lookAt(vec3(0, 4, 6), vec3(0, 0, 0), vec3(0, 1, 0));
	}

	mat4 projection = camera_comp->Projection();
	
	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		component = EntityManager::GetComponent(it->second, mesh_type_);
		mesh = dynamic_pointer_cast<Mesh>(component);

		component = EntityManager::GetComponent(it->second, transform_type_);
		transform = dynamic_pointer_cast<Transform>(component);

		
		
		model = transform->World();
		model_view = view * model;
		mvp = projection * model_view;
		normal = inverse(transpose(mat3(model_view))); // note: always remember that normal_mat needs to be mat3

		shared_ptr<BasicMaterial> material = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);

		vec4 light_pos = material->light_position_;

		// hack to have light move with world
		// todo: implement light as entity
		material->light_position_ = view * root_transform->World() * light_pos;

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