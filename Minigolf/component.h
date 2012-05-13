#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class Entity;

class Component {
public:
	virtual ~Component() { }

	std::string family_name() {
		return family_name_;
	}

protected:
	std::string family_name_;
};

#endif // COMPONENT_H