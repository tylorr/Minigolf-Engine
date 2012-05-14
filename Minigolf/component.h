#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

struct Component {
	std::string family_name;

	Component() : family_name("Component") { }
	Component(std::string family_name) : family_name(family_name) { }
};

#endif // COMPONENT_H