#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "gui_mesh_render.h"
#include "gui_mesh.h"
#include "transform.h"
#include "basic_material.h"

using glm::inverse;
using glm::transpose;
using glm::mat4;
using glm::mat3;
using boost::shared_ptr;

GuiMeshRender::GuiMeshRender(const int &layer) : EntitySystem(layer) {
	TrackType<Transform>();
	TrackType<GuiMesh>();
}

void GuiMeshRender::ProcessEntities(const EntityMap &entities) {

	if (entities.empty()) {
		//printf("Warning: GuiMeshRender: Entity list is empty\n");
		return;
	}

	EntityMap::const_iterator it, ite;

	GuiMeshPtr mesh;
	TransformPtr transform;

	mat4 model, view, projection, model_view, mvp;
	mat3 normal;

	// warning: make sure the values here are all floats
	projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		mesh = gui_mesh_mapper_(it->second);
		transform = transform_mapper_(it->second);

		model = transform->world();
		model_view = model;				// camera does not need to effect this
		mvp = projection * model_view;
		normal = inverse(transpose(mat3(model_view)));

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
