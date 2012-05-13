#include "entity.h"
#include "component.h"

Entity::Entity() {
	Initialize(0);
}

Entity::Entity(const unsigned int &id) {
	Initialize(id);
}

void Entity::Initialize(const unsigned int &id) {
	id_ = id;
	unique_id_ = 0;
	component_index_ = 0;

	Reset();
}

void Entity::Reset() {
	type_bits_ = 0;
	system_bits_ = 0;
	enabled_ = true;
}

void Entity::AddTypeBit(const long &bit) {
	type_bits_ |= bit;
}

void Entity::RemoveTypeBit(const long &bit) {
	type_bits_ &= ~bit;
}

void Entity::AddSystemBit(const long &bit) {
	system_bits_ |= bit;
}

void Entity::RemoveSystemBit(const long &bit) {
	system_bits_ &= ~bit;
}

void Entity::AddComponent(Component *component)
{
	if (component_index_ >= kMaxComponents)
	{
		fprintf(stderr, "ERROR: Entity: Max component limit reached");
		exit(EXIT_FAILURE);
	}

	component->entity_ = this;
	components_[component_index_] = component;
	++component_index_;
};
