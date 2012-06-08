#include "entity.h"
#include "component.h"
#include "lua.hpp"
#include "luabind\luabind.hpp"

unsigned int Entity::next_unique_id_ = 0;

Entity::Entity() : id_(0) {
	Reset();
}

Entity::Entity(const unsigned int &id) : id_(id) {
	Reset();
}

void Entity::Reset() {
	type_bits_ = 0;
	system_bits_ = 0;

	unique_id_ = next_unique_id_++;

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

void Entity::Bind(lua_State *L) {
	luabind::module(L) [
		luabind::class_<Entity, boost::shared_ptr<Entity>>("Entity")
			.property("id", (unsigned int(Entity::*)(void))&Entity::id)
	];
}
