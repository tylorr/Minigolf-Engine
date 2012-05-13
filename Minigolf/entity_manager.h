#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <deque>

#include <boost\shared_ptr.hpp>

class Entity;
class Component;
class ComponentType;

namespace EntityManager {
	typedef boost::shared_ptr<Entity> EntityPtr;
	typedef boost::shared_ptr<Component> ComponenentPtr;

	typedef std::deque<EntityPtr> EntityBag;
	typedef std::deque<ComponenentPtr> ComponentBag;
	typedef std::deque<boost::shared_ptr<ComponentBag>> ComponentByTypeBag;

	boost::shared_ptr<Entity> Create();
	void Remove(const EntityPtr &entity);

	void AddComponent(const EntityPtr &entity, const ComponenentPtr &component);
	void RemoveComponent(const EntityPtr &entity, const ComponenentPtr &component);
	void RemoveComponent(const EntityPtr &entity, const boost::shared_ptr<ComponentType> &type);
}; // class EntityManager

#endif // ENTITY_MANAGER_H