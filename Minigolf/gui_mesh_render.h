#ifndef GUI_MESH_RENDER_H
#define GUI_MESH_RENDER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "component_mapper.h"

//struct Transform;
struct GuiMesh;
struct Transform;

class GuiMeshRender : public EntitySystem {
public:
	GuiMeshRender(const int &layer = 0);

	virtual void ProcessEntities(const EntityMap &entities);

private:
	ComponentMapper<Transform> transform_mapper_;
	ComponentMapper<GuiMesh> gui_mesh_mapper_;
};

#endif // GUI_MESH_RENDER_H