#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
public:
	Entity *Entity;

	virtual ~Component() { }

	virtual void Receive(int message) = 0;
};

#endif // COMPONENT_H