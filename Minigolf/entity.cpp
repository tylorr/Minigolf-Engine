#include "entity.h"
#include "component.h"

void Entity::AddComponent(Component *component)
{
	if (this->componentIndex >= Entity::MAX_COMPONENTS)
	{
		fprintf(stderr, "ERROR: Entity: Max component limit reached");
		exit(EXIT_FAILURE);
	}

	component->Entity = this;
	this->components[this->componentIndex] = component;
	this->componentIndex++;
};

void Entity::Send(int message)
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		if (components[i] != NULL)
		{
			components[i]->Receive(message);
		}
	}
}
