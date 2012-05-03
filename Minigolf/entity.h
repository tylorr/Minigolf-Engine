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
	vec3 position_;

	Entity() {
		position_ = vec3(0.0f);
		component_index_ = 0;
	}

	void AddComponent(Component *component);

	void Send(int message);

private:
	static const int kMaxComponents = 10;

	Component *components_[kMaxComponents];
	unsigned component_index_;
};

#endif // ENTITY_H