#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <string>
#include <vector>

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

class Entity;

class EntitySystem {
	typedef boost::unordered_map<unsigned int, boost::shared_ptr<Entity>> EntityMap;

public:
	int layer_;

	EntitySystem(std::vector<std::string> types, const int &layer = 0);
	~EntitySystem();

	virtual void OnChange(boost::shared_ptr<Entity> entity);

	virtual void Process();

	void set_system_bit(const long &bit) {
		system_bit_ = bit;
	}

	bool operator<(const EntitySystem &other);

protected:
	virtual void Add(boost::shared_ptr<Entity> entity);
	virtual void Remove(boost::shared_ptr<Entity> entity);
	virtual void Enable(boost::shared_ptr<Entity> entity);
	virtual void Disable(boost::shared_ptr<Entity> entity);

	virtual void Begin();
	virtual void End();

	virtual void ProcessEntities(const EntityMap &entities) = 0;

private:
	long system_bit_;
	long type_bits_;

	EntityMap active_entities_;
};

#endif // ENTITY_SYSTEM_H