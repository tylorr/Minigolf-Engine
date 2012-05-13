#include "entity_system.h"
#include "entity.h"
#include "component_type.h"
#include "component_type_manager.h"

using std::string;
using std::vector;
using boost::shared_ptr;

EntitySystem::EntitySystem(vector<string> types) {
	int size = types.size();
	shared_ptr<ComponentType> type;
	vector<string>::iterator it;

	for (it = types.begin(); it != types.end(); ++it) {
		type = ComponentTypeManager::GetTypeFor(*it);
		type_bits_ |= type->bit();
	}
}

EntitySystem::~EntitySystem() {
}

void EntitySystem::Begin() {
}

void EntitySystem::End() {
}

void EntitySystem::Process() {
	Begin();
	ProcessEntities(active_entities_);
	End();
}

void EntitySystem::OnChange(shared_ptr<Entity> entity) {
	bool contains = (system_bit_ & entity->system_bits()) == system_bit_;
	bool interest = (type_bits_ & entity->type_bits()) == type_bits_;

	if (interest && !contains) {
		Add(entity);
	} else if (!interest && contains) {
		Remove(entity);
	} else if (interest && contains) {
		if (entity->enabled()) {
			Enable(entity);
		} else {
			Disable(entity);
		}
	}
}

void EntitySystem::Add(shared_ptr<Entity> entity) {
	entity->AddSystemBit(system_bit_);

	if (entity->enabled()) {
		Enable(entity);
	}
}

void EntitySystem::Remove(shared_ptr<Entity> entity) {
	entity->RemoveSystemBit(system_bit_);
	if (entity->enabled()) {
		Disable(entity);
	}
}

void EntitySystem::Enable(shared_ptr<Entity> entity) {
	EntityMap::iterator it;

	it = this->active_entities_.find(entity->id());
	if (it != active_entities_.end()) {
		return;
	}

	active_entities_[entity->id()] = entity;
}

void EntitySystem::Disable(shared_ptr<Entity> entity) {
	EntityMap::iterator it;

	it = this->active_entities_.find(entity->id());
	if (it == active_entities_.end()) {
		return;
	}

	active_entities_.erase(entity->id());
}