#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

struct Component {
	std::string family_name;

	Component(std::string family_name) : family_name(family_name) { }

	virtual ~Component() { }
};

#endif // COMPONENT_H