#include "luabind\luabind.hpp"

#include "script_system.h"
#include "script.h"

ScriptSystem::ScriptSystem(lua_State *L, const int &layer) 
	: EntitySystem(layer),
	  L(L) {
		  TrackType<Script>();
}

void ScriptSystem::Init() {
	EntityMap::iterator it, ite;
	ScriptPtr script;
	EntityPtr entity;

	for (it = active_entities_.begin(), ite = active_entities_.end(); it != ite; ++it) {
		entity = it->second;
		script = script_mapper_(entity);

		luaL_dofile(L, script->file);

		luabind::call_function<void>(L, "Init");
	}
}

void ScriptSystem::Resolve() {
	EntityMap::iterator it, ite;
	ScriptPtr script;
	EntityPtr entity;

	for (it = active_entities_.begin(), ite = active_entities_.end(); it != ite; ++it) {
		entity = it->second;
		script = script_mapper_(entity);

		luaL_dofile(L, script->file);

		luabind::call_function<void>(L, "Resolve");
	}
}

void ScriptSystem::ProcessEntities(const EntityMap &entities) {
	EntityMap::const_iterator it, ite;
	ScriptPtr script;
	EntityPtr entity;

	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		entity = it->second;
		script = script_mapper_(entity);

		luaL_dofile(L, script->file);

		luabind::call_function<void>(L, "Update");
	}
}