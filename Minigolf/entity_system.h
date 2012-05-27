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

	EntitySystem(const int &layer = 0) : layer_(layer), type_bits_(0) { }
	~EntitySystem();

	virtual void OnChange(const EntityPtr &entity);

	virtual void Init() { }

	virtual void Resolve() { }

	virtual void Process();

	void set_system_bit(const long &bit) {
		system_bit_ = bit;
	}

	bool operator<(const EntitySystem &other);

protected:
	typedef boost::unordered_map<unsigned int, EntityPtr> EntityMap;

	void TrackType(const type_info &type);

	template <typename T>
	void TrackType() {
		const type_info &type = typeid(T);
		TrackType(type);
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