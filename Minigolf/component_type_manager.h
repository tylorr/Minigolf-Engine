#ifndef COMPONENT_TYPE_MANAGER_H
#define COMPONENT_TYPE_MANAGER_H

#include <string>

#include <boost\shared_ptr.hpp>

class ComponentType;

namespace ComponentTypeManager {

	boost::shared_ptr<ComponentType> GetTypeFor(const std::string &family_name);

}; // namespace ComponentTypeManager

#endif // COMPONENT_TYPE_MANAGER_H