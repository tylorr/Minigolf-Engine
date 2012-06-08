#ifndef RENDERER_H
#define RENDERER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "component_mapper.h"
#include "transform.h"

//struct Transform;
struct Mesh;
struct Camera;

class RenderSystem : public EntitySystem {
public:
	RenderSystem(const int &layer = 0);

	virtual void OnChange(const EntityPtr &entity);

	virtual void ProcessEntities(const EntityMap &entities);

	//virtual bool CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity);

private:
	ComponentMapper<Transform> transform_mapper_;
	ComponentMapper<Mesh> mesh_mapper_;
	ComponentMapper<Camera> camera_mapper_;

	boost::shared_ptr<Entity> camera_;
};

#endif // RENDERER_H