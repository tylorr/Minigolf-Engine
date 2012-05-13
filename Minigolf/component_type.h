#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

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