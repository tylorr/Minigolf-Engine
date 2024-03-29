#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <string>

#include <boost\shared_ptr.hpp>

#include "entity.h"

class EntitySystem;

namespace SystemManager {
	void AddSystem(const boost::shared_ptr<EntitySystem> &system);
	long GetBitFor(const boost::shared_ptr<EntitySystem> &system);

	void Refresh(const EntityPtr &entity);

	void Init();
	void Resolve();
	void Update();
	void ReloadScript();
}; // namespace SystemManager

#endif // SYSTEM_MANAGER_H