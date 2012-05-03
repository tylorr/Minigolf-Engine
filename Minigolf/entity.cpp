#include "entity.h"
#include "component.h"

void Entity::AddComponent(Component *component)
{
	if (component_index_ >= kMaxComponents)
	{
		fprintf(stderr, "ERROR: Entity: Max component limit reached");
		exit(EXIT_FAILURE);
	}

	component->Entity = this;
	components_[component_index_] = component;
	++component_index_;
};

void Entity::Send(int message)
{
	for (int i = 0; i < kMaxComponents; ++i)
	{
		if (components_[i] != NULL)
		{
			components_[i]->Receive(message);
		}
	}
}
