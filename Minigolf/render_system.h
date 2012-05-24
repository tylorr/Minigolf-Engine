#ifndef RENDERER_H
#define RENDERER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"

class ComponentType;
struct Transform;
struct Camera;

class RenderSystem : public EntitySystem {
public:

	RenderSystem();
	~RenderSystem();

	virtual void ProcessEntities(const EntityMap &entities);

private:
	boost::shared_ptr<ComponentType> mesh_type_;
	boost::shared_ptr<ComponentType> transform_type_;
	
};

#endif // RENDERER_H