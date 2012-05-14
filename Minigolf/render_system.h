#ifndef RENDERER_H
#define RENDERER_H

#include "entity_system.h"

class RenderSystem : public EntitySystem {
public:
	RenderSystem();
	~RenderSystem();

	virtual void ProcessEntities(const EntityMap &entities);
};

#endif // RENDERER_H