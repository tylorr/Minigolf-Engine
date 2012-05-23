#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <deque>
#include <string>

#include <boost\shared_ptr.hpp>

#include "component_type_manager.h"

class Entity;
struct Component;
class ComponentType;

namespace EntityManager {
	typedef boost::shared_ptr<Entity> EntityPtr;
	typedef boost::shared_ptr<Component> ComponentPtr;

	typedef std::deque<EntityPtr> EntityBag;
	typedef std::deque<ComponentPtr> ComponentBag;
	typedef std::deque<boost::shared_ptr<ComponentBag>> ComponentByTypeBag;

	EntityPtr Create();
	void Remove(const EntityPtr &entity);

	void AddComponent(const EntityPtr &entity, const ComponentPtr &component);
	void RemoveComponent(const EntityPtr &entity, const ComponentPtr &component);
	void RemoveComponent(const EntityPtr &entity, const boost::shared_ptr<ComponentType> &type);

	ComponentPtr GetComponent(const EntityPtr &entity, const boost::shared_ptr<ComponentType> &type);
	ComponentPtr GetComponent(const EntityPtr &entity, const std::string &family_name);

	template <typename T>
	boost::shared_ptr<T> GetComponent(const EntityPtr &entity, const std::string &family_name) {
		return boost::dynamic_pointer_cast<T>(GetComponent(entity, ComponentTypeManager::GetTypeFor(family_name)));
	}

	void Register(const EntityPtr &entity, const std::string &name);
	EntityPtr Find(const std::string &name);

}; // class EntityManager

#endif // ENTITY_MANAGER_H