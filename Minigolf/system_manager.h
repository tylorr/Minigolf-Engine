#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <boost\shared_ptr.hpp>

class EntitySystem;

namespace SystemManager {
	void AddSystem(const boost::shared_ptr<EntitySystem> &system);
	long GetBitFor(const boost::shared_ptr<EntitySystem> &system);
}; // namespace SystemManager

#endif // SYSTEM_MANAGER_H