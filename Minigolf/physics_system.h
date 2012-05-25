#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include "glm\glm.hpp"
#include "entity_system.h"

class ComponentType;

class PhysicSystem : public EntitySystem {
public:
	PhysicsSystem();
	~PhysicsSystem();
}

#endif