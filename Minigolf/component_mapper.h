#ifndef COMPONENT_MAPPER_H
#define COMPONENT_MAPPER_H

#include <boost\shared_ptr.hpp>

class ComponentType;
class Entity;

template <typename T>
class ComponentMapper {
public:
	ComponentMapper();

	boost::shared_ptr<T> Get(boost::shared_ptr<Entity> entity);

private:
	boost::shared_ptr<ComponentType> type_;
};

#endif // COMPONENT_MAPPER_H
