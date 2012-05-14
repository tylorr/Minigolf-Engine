#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <deque>

#include <boost\shared_ptr.hpp>

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

	ComponentPtr GetComponent(EntityPtr entity, boost::shared_ptr<ComponentType> type);

}; // class EntityManager

#endif // ENTITY_MANAGER_H