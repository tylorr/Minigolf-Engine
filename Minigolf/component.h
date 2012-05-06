#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
public:
	Entity *entity_;

	virtual ~Component() { }

	virtual void Receive(int message) = 0;
};

#endif // COMPONENT_H