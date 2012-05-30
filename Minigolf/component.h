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
	/*
		remarks:	Component inheritance must be polymorphic i.e. virtual
					inheritance. Otherwise a typeid() will return Component
					and not derived if it is used on a Component pointer to
					a derived object.
	*/
	virtual ~Component() { }
};

typedef boost::shared_ptr<Component> ComponentPtr;

#endif // COMPONENT_H