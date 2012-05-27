#ifndef COMPONENT_TYPE_MANAGER_H
#define COMPONENT_TYPE_MANAGER_H

#include <string>

#include <boost\shared_ptr.hpp>

#include "component_type.h"

/*
	remarks:	ComponentTypeManager stores the relation of from Component
				family_names to ComponentTypes
*/
namespace ComponentTypeManager {

	/*
		input:		A string representing the family_name of a Component. A new 
					family_name - ComponentType relation is created if one does 
					not exist

		output:		A ComponentType that represents the family_name
	*/
	ComponentTypePtr GetTypeFor(const std::string &family_name);

}; // namespace ComponentTypeManager

#endif // COMPONENT_TYPE_MANAGER_H