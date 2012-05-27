#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <string>
#include <vector>
#include <typeinfo.h>

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "entity.h"

class EntitySystem {
public:
	int layer_;

	/*
		remarks:	Use the constructor to clear pointers and set default values
	*/
	EntitySystem(const int &layer = 0) : layer_(layer), type_bits_(0) { }

	/*
		remarks:	Use the destructor to make sure that all memory has been cleared
	*/
	~EntitySystem();

	/*
		remarks:	Use this stage to configure data (usually loaded from a level)
					Overload this to give it your own parameters, but make sure that
					you call it yourself.
	*/
	virtual void Init() { }

	/*
		remarks:	Use this stage to make any inter-system connections
	*/
	virtual void Resolve() { }

	/*
		remarks:	Use this stage to clear memory and release assets
	*/
	virtual void Deinit() { }

	virtual void Process();

	virtual void OnChange(const EntityPtr &entity);

	void set_system_bit(const long &bit) {
		system_bit_ = bit;
	}

	bool operator<(const EntitySystem &other);

protected:
	typedef boost::unordered_map<unsigned int, EntityPtr> EntityMap;

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

private:
	long system_bit_;
	long type_bits_;

	EntityMap active_entities_;
};

#endif // ENTITY_SYSTEM_H