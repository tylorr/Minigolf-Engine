#ifndef ENTITY_H
#define ENTITY_H

#include "glm\glm.hpp"

using glm::vec3;

class Component;


// TODO: Entities should have a Transform object describing 
// translation rotation and scale

// TODO: Entities should have children and parents

// TODO: Find out better way to store vertices and indices
// in abstract way


class Entity {
public:
	vec3 Position;

	Entity() {
		Position = vec3(0.0f);
		componentIndex = 0;
	}

	void AddComponent(Component *component);

	void Send(int message);

private:
	static const int MAX_COMPONENTS = 10;

	Component *components[MAX_COMPONENTS];
	unsigned componentIndex;
};

#endif // ENTITY_H