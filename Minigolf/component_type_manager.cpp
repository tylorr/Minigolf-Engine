#include <boost\unordered_map.hpp>

#include "component_type_manager.h"
#include "component_type.h"

namespace ComponentTypeManager {

using std::string;
using boost::shared_ptr;

namespace {
	typedef boost::unordered_map<string, shared_ptr<ComponentType>> ComponentTypeMap;
	ComponentTypeMap component_types_;
};

shared_ptr<ComponentType> GetTypeFor(const string &family_name) {
	shared_ptr<ComponentType> type;
	ComponentTypeMap::iterator it;
	
	it = component_types_.find(family_name);
	if (it == component_types_.end()) {
		type = shared_ptr<ComponentType>(new ComponentType());
		component_types_[family_name] = type;
	} else {
		type = it->second;
	}

	return type;
}


};