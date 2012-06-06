	#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <deque>
#include <string>
#include <typeinfo.h>

#include <boost\shared_ptr.hpp>
#include <boost\type_traits\is_base_of.hpp>

#include "component_type_manager.h"
#include "entity.h"
#include "component.h"
#include "component_type.h"

/*
	remarks:	EntityManager stores relations between Entities, Components,
				and Entity names.
*/
namespace EntityManager {
	typedef std::deque<EntityPtr> EntityBag;
	typedef std::deque<ComponentPtr> ComponentBag;
	typedef boost::shared_ptr<ComponentBag> ComponentBagPtr;
	typedef std::deque<ComponentBagPtr> ComponentByTypeBag;

	void Destroy();

	/*
		output:		A new empty Entity with new unique_id

		remarks:	Use this to create any new Entities
	*/
	EntityPtr Create();

	void Remove(const EntityPtr &entity);
	void RemoveAll();

	/*
		remarks:	Binds a component to an Entity. 
	*/
	void AddComponent(const EntityPtr &entity, const ComponentPtr &component);

	void RemoveComponentsOfEntity(const EntityPtr &entity);

	void RemoveComponent(const EntityPtr &entity, const ComponentPtr &component);

	void RemoveComponent(const EntityPtr &entity, const ComponentTypePtr &type);

	boost::shared_ptr<Component> GetComponent(const EntityPtr &entity, const ComponentTypePtr &comp_type);

	template <typename T>
	boost::shared_ptr<T> GetComponent(const EntityPtr &entity, const ComponentTypePtr &comp_type) {
		static_assert(
			(boost::is_base_of<Component, T>::value),
			"T must be a descendant of Component"
		);

		return boost::dynamic_pointer_cast<T>(GetComponent(entity, comp_type));
	}

	template <typename T>
	boost::shared_ptr<T> GetComponent(const EntityPtr &entity) {
		return GetComponent<T>(entity, ComponentTypeManager::GetTypeFor<T>());
	}

	/*
		input:		Entity and the desired name to associate with it

		remarks:	Use this to attach a name to an entity.
	*/
	void Register(const EntityPtr &entity, const std::string &name);

	/*
		input:		Name of Entity being looked for

		output:		ptr to Entity with input name, EntityPtr() 
					if an Entity with that name does not exist.

		remarks:	Use this to find an Entity by name. Make sure to that
					the Entity has been registered first  using:
					EntityManager::Register()
	*/
	EntityPtr Find(const std::string &name);

}; // class EntityManager

#endif // ENTITY_MANAGER_H