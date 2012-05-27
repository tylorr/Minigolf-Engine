#ifndef RENDERER_H
#define RENDERER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "component_type.h"

struct Transform;
struct Camera;

class RenderSystem : public EntitySystem {
public:
	bool relative_;
	glm::vec3 reference_;
	glm::vec3 up_;

	RenderSystem();
	~RenderSystem();

	virtual void ProcessEntities(const EntityMap &entities);

	//virtual bool CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity);

private:
	ComponentTypePtr mesh_type_;
	ComponentTypePtr transform_type_;
	
};

#endif // RENDERER_H