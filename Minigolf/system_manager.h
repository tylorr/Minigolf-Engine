#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <string>

#include <boost\shared_ptr.hpp>

class Entity;
class EntitySystem;

namespace SystemManager {
	void AddSystem(const boost::shared_ptr<EntitySystem> &system);
	long GetBitFor(const std::string &family_name);

	void Refresh(const boost::shared_ptr<Entity> &entity);
	void Update();
}; // namespace SystemManager

#endif // SYSTEM_MANAGER_H