#ifndef COMPONENT_MAPPER_H
#define COMPONENT_MAPPER_H

#include <boost\shared_ptr.hpp>
#include <boost\type_traits\is_base_of.hpp>

#include "component.h"
#include "entity_manager.h"
#include "component_type_manager.h"

class ComponentType;
class Entity;

template <typename T>
class ComponentMapper {
public:
	ComponentMapper() {
		static_assert(
			(boost::is_base_of<Component, T>::value),
			"T must be a descendant of Component"
		);

		type_ = ComponentTypeManager::GetTypeFor<T>();
	}

	boost::shared_ptr<T> Get(const boost::shared_ptr<Entity> &entity) {
		return EntityManager::GetComponent<T>(entity, type_);
	}

	boost::shared_ptr<T> operator()(const boost::shared_ptr<Entity> &entity) {
		return Get(entity);
	}

private:
	boost::shared_ptr<ComponentType> type_;
};

#endif // COMPONENT_MAPPER_H
