#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "gui_mesh_render.h"
#include "gui_mesh.h"
#include "transform.h"

using glm::inverse;
using glm::transpose;
using glm::mat4;
using glm::mat3;

GuiMeshRender::GuiMeshRender(const int &layer) : EntitySystem(layer) {
	TrackType<Transform>();
	TrackType<GuiMesh>();
}

void GuiMeshRender::ProcessEntities(const EntityMap &entities) {

	if (entities.empty()) {
		printf("Warning: GuiMeshRender: Entity list is empty\n");
		return;
	}

	EntityMap::const_iterator it, ite;

	GuiMeshPtr mesh;
	TransformPtr transform;

	mat4 model, view, projection, model_view, mvp;
	mat3 normal;

	projection = glm::ortho(0, 800, 600, 0);
}
