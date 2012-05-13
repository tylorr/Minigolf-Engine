#include "entity_manager.h"

#include "entity.h"
#include "component.h"
#include "component_type.h"
#include "component_type_manager.h"

using boost::shared_ptr;

namespace EntityManager {

// kind of like private static variables
namespace {
	int next_id_;
	int next_unique_id_;

	EntityBag active_entities_;
	EntityBag inactive_entities_;
	ComponentByTypeBag components_by_type_;
};

shared_ptr<Entity> Create() {
	// temp
	EntityPtr entity;

	// no entities for re-use?
	if (inactive_entities_.empty())
	{
		// create new entity with plain id
		entity = EntityPtr(new Entity(next_id_++));
	}
	else
	{
		// grab existing entity
		entity = inactive_entities_.back();
		inactive_entities_.pop_back();

		// reset bits
		entity->Reset();
	}

	// set unique_id, plain id is not unique to new entities
	entity->unique_id_ = next_unique_id_++;

	
	// is there not enough room?
	if (entity->id() >= active_entities_.size()) {
		// expand
		active_entities_.resize(entity->id() * 2 + 1);
	} 

	active_entities_[entity->id()] = entity;

	// todo: increment count

	return entity;
}

void Remove(const EntityPtr &entity) {
	// remove entity from bag
	active_entities_[entity->id()] = EntityPtr();

	// this may not be necessary
	entity->RemoveTypeBit(~0);

	// todo: notify systems
	// todo: decrement count

	// add entity to inactive list for later use
	inactive_entities_.push_back(entity);
}

void AddComponent(const EntityPtr &entity, const ComponenentPtr &component) {
	shared_ptr<ComponentType> type = ComponentTypeManager::GetTypeFor(component->family_name());
	shared_ptr<ComponentBag> components;

	// type not in bag?
	if (type->id() >= components_by_type_.size()) {
		components_by_type_.resize(type->id() * 2 + 1);
	}

	if (components_by_type_[type->id()] == shared_ptr<ComponentBag>())
	{
		components = shared_ptr<ComponentBag>(new ComponentBag());
		components_by_type_[type->id()] = components;
	} else {
		// grab existing type from bag
		components = components_by_type_[type->id()];
	}

	// link component and entity
	if (entity->id() >= components->size())
	{
		components->resize(entity->id() * 2 + 1);
	}
	(*components)[entity->id()] = component;

	// add type of component to entity
	entity->AddTypeBit(type->bit());
}

void RemoveComponent(const EntityPtr &entity, const ComponenentPtr &component) {
	// find type from component
	shared_ptr<ComponentType> type = ComponentTypeManager::GetTypeFor(component->family_name());
	RemoveComponent(entity, type);
}

void RemoveComponent(const EntityPtr &entity, const shared_ptr<ComponentType> &type) {
	// get list of components
	shared_ptr<ComponentBag> components = components_by_type_[type->id()];

	// remove component/entity relationship
	(*components)[entity->id()] = ComponenentPtr();

	// remove bit from entity
	entity->RemoveTypeBit(type->bit());
}

}; // namespace EntityManager