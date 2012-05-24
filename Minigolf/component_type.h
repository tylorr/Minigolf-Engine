#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

/*
	remarks:	ComponentType stores information about the "type" of a component.
				There is a unique mapping from the family name from a Component to 
				a ComponentType. Each ComponentType has a unique ID and bit in a 
				bit string.	The ID is used to map Component to Entities in 
				EntityManager. The bit is used to keep track of which Components 
				an Entity contains.
*/
class ComponentType {
public:
	ComponentType();

	unsigned int id();
	long bit();

private:

	static long next_bit_;
	static unsigned int next_id_;

	long bit_;
	unsigned int id_;
};

#endif // COMPONENT_TYPE_H