#ifndef COMPONENT_TYPE_MANAGER_H
#define COMPONENT_TYPE_MANAGER_H

#include <typeinfo.h>

#include <boost\shared_ptr.hpp>
#include <boost\type_traits\is_base_of.hpp>

class ComponentType;
struct Component;

/*
	remarks:	ComponentTypeManager stores the relation of from Component
				family_names to ComponentTypes
*/
namespace ComponentTypeManager {

	namespace Inner {
		boost::shared_ptr<ComponentType> GetTypeFor(const type_info &type);
	};

	boost::shared_ptr<ComponentType> GetTypeFor(const boost::shared_ptr<Component> &component);

	template <typename T>
	boost::shared_ptr<ComponentType> GetTypeFor() {
		static_assert(
			(boost::is_base_of<Component, T>::value),
			"T must be a descendant of Component"
		);

		const type_info &type = typeid(T);
		return Inner::GetTypeFor(type);
	}

}; // namespace ComponentTypeManager

#endif // COMPONENT_TYPE_MANAGER_H