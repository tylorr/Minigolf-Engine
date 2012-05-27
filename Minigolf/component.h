#ifndef COMPONENT_H
#define COMPONENT_H

//#include <string>
#include <boost\shared_ptr.hpp>

/*
	remarks:	Base class for components. All components should extend this.
				Components generally should only contain data, not logic. 
				Logic in components should only serve to provide data in 
				different formats.
*/
struct Component {
	//std::string family_name;

	/*
		remarks:	Convention is to use CamelCase for family_name that 
					matches the struct name of the component.
	*/
	//Component(const std::string &family_name) : family_name(family_name) { }

	virtual ~Component() { }
};

typedef boost::shared_ptr<Component> ComponentPtr;

#endif // COMPONENT_H