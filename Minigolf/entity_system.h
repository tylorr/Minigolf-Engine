#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <string>
#include <vector>
#include <typeinfo.h>
#include "lua.hpp"
#include "luabind\luabind.hpp"

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "entity.h"

class EntitySystem {
public:
	int layer_;
	std::string script_;

	/*
		remarks:	Use the constructor to clear pointers and set default values
	*/
	EntitySystem(const int &layer) : layer_(layer), script_(""), has_script_(false), type_bits_(0), system_bit_(0), L(NULL) { }
	EntitySystem(const int &layer, const std::string &script) : layer_(layer), script_(script), type_bits_(0), system_bit_(0), L(NULL) {
		if (script_.compare("") != 0) {
			has_script_ = true;
		}
	}

	/*
		remarks:	Use the destructor to make sure that all memory has been cleared
	*/
	virtual ~EntitySystem() { }

	/*
		remarks:	Use this stage to configure data (usually loaded from a level)
					Overload this to give it your own parameters, but make sure that
					you call it yourself.
	*/
	virtual void Init();

	/*
		remarks:	Use this stage to make any inter-system connections
	*/
	virtual void Resolve() { }

	virtual void Process();

	virtual void OnChange(const EntityPtr &entity);

	virtual void ReloadScript();

	void set_system_bit(const long &bit) {
		system_bit_ = bit;
	}

	bool operator<(const EntitySystem &other);

protected:
	typedef boost::unordered_map<unsigned int, EntityPtr> EntityMap;

	bool has_script_;

	EntityMap active_entities_;
	lua_State *L; 

	template <typename T>
	void TrackType() {
		ComponentTypePtr comp_type = ComponentTypeManager::GetTypeFor<T>();
		AddTypeBit(comp_type->bit());
	}

	void AddTypeBit(const long &bit);

	virtual bool CheckEntity(const bool &interest, const bool &contains, const EntityPtr &entity) { return true; }

	virtual void Add(const EntityPtr &entity);
	virtual void Remove(const EntityPtr &entity);
	virtual void Enable(const EntityPtr &entity);
	virtual void Disable(const EntityPtr &entity);

	virtual void Begin() { }
	virtual void End() { }

	virtual void ProcessEntities(const EntityMap &entities) { }

	template<typename T>
	T GetAttribute(const char *name) {
		try {
			return luabind::object_cast<T>(luabind::globals(L)[name]);
		} catch (const luabind::cast_failed &error) {
			printf("Error: Attribute \"%s\" does not exist: %s", name, error.what());
			throw;
		}
	}

private:
	long system_bit_;
	long type_bits_;
};

#endif // ENTITY_SYSTEM_H