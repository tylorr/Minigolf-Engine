#include <boost\type_traits\is_base_of.hpp>

#include "component_mapper.h"
#include "component.h"
#include "entity.h"
#include "entity_manager.h"
#include "component_type_manager.h"

template <typename T>
ComponentMapper<T>::ComponentMapper() {
	static_assert(
		(boost::is_base_of<Component, T>::value),
		"T must be a descendant of Component"
	);

	type_ = ComponentTypeManager::GetTypeFor<T>();
}

template <typename T>
boost::shared_ptr<T> ComponentMapper<T>::Get(EntityPtr entity) {
	return EntityManager::GetComponent(entity, type_);
}