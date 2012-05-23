#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <string>
#include <vector>

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

class Entity;

class EntitySystem {
public:
	int layer_;

	EntitySystem(const int &layer = 0) : layer_(layer), type_bits_(0) { }
	~EntitySystem();

	virtual void OnChange(const boost::shared_ptr<Entity> &entity);

	virtual void Init() { }

	virtual void Resolve() { }

	virtual void Process();

	void set_system_bit(const long &bit) {
		system_bit_ = bit;
	}

	std::string family_name() {
		return family_name_;
	}

	bool operator<(const EntitySystem &other);

protected:
	typedef boost::unordered_map<unsigned int, boost::shared_ptr<Entity>> EntityMap;

	std::string family_name_;

	void AddTypeByName(const std::string &family_name);
	void AddTypeBit(const long &bit);

	virtual bool CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity) { return true; }

	virtual void Add(const boost::shared_ptr<Entity> &entity);
	virtual void Remove(const boost::shared_ptr<Entity> &entity);
	virtual void Enable(const boost::shared_ptr<Entity> &entity);
	virtual void Disable(const boost::shared_ptr<Entity> &entity);

	virtual void Begin() { }
	virtual void End() { }

	virtual void ProcessEntities(const EntityMap &entities) { }

private:
	long system_bit_;
	long type_bits_;

	EntityMap active_entities_;
};

#endif // ENTITY_SYSTEM_H