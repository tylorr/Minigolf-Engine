#include <boost\unordered_map.hpp>
#include <typeinfo.h>

#include "entity_manager.h"
#include "entity.h"
#include "component.h"
#include "component_type.h"
#include "component_type_manager.h"
#include "system_manager.h"

using boost::shared_ptr;
using boost::unordered_map;
using std::string;

namespace EntityManager {

// kind of like private static variables
namespace {
	int next_id_;
	int next_unique_id_;

	EntityBag active_entities_;
	EntityBag inactive_entities_;
	ComponentByTypeBag components_by_type_;

	unordered_map<string, EntityPtr> entity_names_;
};

void Destroy() {
	EntityBag::iterator it, ite;
	EntityPtr entity;

	for (it = active_entities_.begin(), ite = active_entities_.end(); it != ite; ++it) {
		entity = *it;
		if (entity) {
			Remove(entity);
		}
	}

	active_entities_.clear();
	inactive_entities_.clear();
	components_by_type_.clear();
	entity_names_.clear();
}

EntityPtr Create() {
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
	
	// is there not enough room?
	if (entity->id() >= active_entities_.size()) {
		// expand
		active_entities_.resize(entity->id() * 2 + 1);
	} 

	active_entities_[entity->id()] = entity;

	// todo: increment count
	SystemManager::Refresh(entity);

	return entity;
}

void Remove(const EntityPtr &entity) {
	// remove entity from bag
	active_entities_[entity->id()].reset();
	entity_names_[entity->name].reset();

	entity->RemoveTypeBit(~0);

	SystemManager::Refresh(entity);
	
	RemoveComponentsOfEntity(entity);

	// add entity to inactive list for later use
	inactive_entities_.push_back(entity);
}

void RemoveAll() {
	EntityBag::iterator it, ite;
	EntityPtr entity;

	for (it = active_entities_.begin(), ite = active_entities_.end(); it != ite; ++it) {
		entity = *it;
		if (entity) {
			Remove(entity);
		}
	}
}

void AddComponent(const EntityPtr &entity, const ComponentPtr &component) {
	ComponentTypePtr type = ComponentTypeManager::GetTypeFor(component);
	ComponentBagPtr components;

	// type not in bag?
	if (type->id() >= components_by_type_.size()) {
		components_by_type_.resize(type->id() * 2 + 1);
	}

	if (components_by_type_[type->id()] == ComponentBagPtr())
	{
		components = ComponentBagPtr(new ComponentBag());
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

	SystemManager::Refresh(entity);
}

void RemoveComponentsOfEntity(const EntityPtr &entity) {
	ComponentByTypeBag::iterator it;
	ComponentByTypeBag::iterator ite;
	ComponentBagPtr components;
	ComponentPtr component;

	unsigned int id = entity->id();

	for (it = components_by_type_.begin(), ite = components_by_type_.end(); it != ite; ++it) {
		components = *it;
		if (components && id < components->size()) {
			component = (*components)[id];

			if (component) {
				component->Deinit();
				component.reset();
			}
		}
	}
}

void RemoveComponent(const EntityPtr &entity, const ComponentPtr &component) {
	// find type from component
	ComponentTypePtr type = ComponentTypeManager::GetTypeFor(component);
	RemoveComponent(entity, type);
}

void RemoveComponent(const EntityPtr &entity, const ComponentTypePtr &type) {
	// get list of components
	ComponentBagPtr components = components_by_type_[type->id()];
	ComponentPtr component;

	component = (*components)[entity->id()];

	// remove component/entity relationship
	component->Deinit();
	component.reset();

	// remove bit from entity
	entity->RemoveTypeBit(type->bit());

	SystemManager::Refresh(entity);
}

ComponentPtr GetComponent(const EntityPtr &entity, const ComponentTypePtr &comp_type) {
	ComponentBagPtr bag; 
	ComponentPtr component;
	
	if (comp_type->id() < components_by_type_.size()) {

		bag = components_by_type_[comp_type->id()];

		if (entity->id() < bag->size()) {
			component = (*bag)[entity->id()];
		}
	}

	return component;
}



void Register(const EntityPtr &entity, const std::string &name) {
	entity_names_[name] = entity;
	entity->name = name;
}

EntityPtr Find(const string &name) {
	return entity_names_[name];
}

}; // namespace EntityManager