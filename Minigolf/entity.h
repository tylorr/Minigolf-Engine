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

#include "transform.h"


class Entity {
public:
	Transform transform_;
	unsigned int unique_id_;
	
	Entity();
	Entity(const unsigned int &id);

	void Initialize(const unsigned int &id);
	void Reset();

	unsigned int id() {
		return id_;
	}

	long system_bits() {
		return system_bits_;
	}

	long type_bits() {
		return type_bits_;
	}

	bool enabled() {
		return enabled_;
	}

	void set_enabled(const bool &enabled) {
		enabled_ = enabled;
	}

	void AddTypeBit(const long &bit);
	void RemoveTypeBit(const long &bit);
	void AddSystemBit(const long &bit);
	void RemoveSystemBit(const long &bit);

	void AddComponent(Component *component);

private:
	static const int kMaxComponents = 10;

	bool enabled_;

	unsigned int id_;

	long system_bits_;
	long type_bits_;

	Component *components_[kMaxComponents];
	unsigned int component_index_;
};

#endif // ENTITY_H