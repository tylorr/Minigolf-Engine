#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "lua.hpp"

#include "entity_system.h"
#include "component_mapper.h"

struct Script;

class ScriptSystem : public EntitySystem {
public:
	ScriptSystem(lua_State *L, const int &layer = 0);

	virtual void Init();
	virtual void Resolve();

	virtual void ProcessEntities(const EntityMap &entities);

private:
	lua_State *L;
	ComponentMapper<Script> script_mapper_;
};

#endif // SCRIPT_SYSTEM_H