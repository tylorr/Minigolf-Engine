#include <boost\unordered_map.hpp>

#include "component_type_manager.h"
#include "component_type.h"
#include "component.h"

namespace ComponentTypeManager {

namespace {
	typedef boost::unordered_map<const type_info *, ComponentTypePtr> ComponentTypeMap;

	ComponentTypeMap component_types_;
};

namespace Inner {
	ComponentTypePtr GetTypeFor(const type_info &type) {
		//make sure we are not passing something from boost i.e shared_ptr
		//assert(!strstr(type.name(), "boost"));

		ComponentTypePtr comp_type;
		ComponentTypeMap::iterator it;
	
		it = component_types_.find(&type);
		if (it == component_types_.end()) {
			comp_type = ComponentTypePtr(new ComponentType());
			component_types_[&type] = comp_type;
		} else {
			comp_type = it->second;
		}

		return comp_type;
	}						
};

ComponentTypePtr GetTypeFor(const ComponentPtr &component) {
	const type_info &ti = typeid(*component);
	return Inner::GetTypeFor(ti);
}
};