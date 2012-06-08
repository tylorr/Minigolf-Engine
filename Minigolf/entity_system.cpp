#include <cstdarg>

#include "entity_system.h"
#include "entity.h"
#include "component_type.h"
#include "component_type_manager.h"

using std::string;
using std::vector;
using boost::shared_ptr;

void EntitySystem::ReloadScript() {
	if (L == NULL) {
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	luabind::open(L);

	luaL_dofile(L, script_.c_str());
}

void EntitySystem::AddTypeBit(const long &bit) {
	type_bits_ |= bit;
}

void EntitySystem::Init() {
}

void EntitySystem::Process() {
	Begin();
	ProcessEntities(active_entities_);
	End();
}

void EntitySystem::OnChange(const EntityPtr &entity) {
	bool contains = (system_bit_ & entity->system_bits()) == system_bit_;
	bool interest = (type_bits_ & entity->type_bits()) == type_bits_;

	if (type_bits_ > 0) {
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
}

void EntitySystem::Add(const EntityPtr &entity) {
	entity->AddSystemBit(system_bit_);

	if (entity->enabled()) {
		Enable(entity);
	}
}

void EntitySystem::Remove(const EntityPtr &entity) {
	entity->RemoveSystemBit(system_bit_);
	if (entity->enabled()) {
		Disable(entity);
	}
}

void EntitySystem::Enable(const EntityPtr &entity) {
	EntityMap::iterator it;

	it = this->active_entities_.find(entity->id());
	if (it != active_entities_.end()) {
		return;
	}

	active_entities_[entity->id()] = entity;
}

void EntitySystem::Disable(const EntityPtr &entity) {
	EntityMap::iterator it;

	it = this->active_entities_.find(entity->id());
	if (it == active_entities_.end()) {
		return;
	}

	active_entities_.erase(entity->id());
}

bool EntitySystem::operator<(const EntitySystem &other) {
	return layer_ < other.layer_;
}