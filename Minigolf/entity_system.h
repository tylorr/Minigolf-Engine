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
	EntitySystem(std::vector<std::string> types);
	~EntitySystem();

	virtual void OnChange(boost::shared_ptr<Entity> entity);

	virtual void Process();

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