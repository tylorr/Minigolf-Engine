#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "glm\gtc\quaternion.hpp"

#include "component.h"

using std::vector;

using glm::vec3;
using glm::quat;
using glm::mat4;

struct Transform : public Component {
public:
	vec3 position;
	vec3 scale;
	quat rotation;

	boost::shared_ptr<Transform> parent;
	vector<boost::shared_ptr<Transform>> children;

	Transform() : Component("Transform"), dirty_(true) { }

	mat4 Local() const;
	mat4 World();

private:
	bool dirty_;
	mat4 world_;
	mat4 cached_parent_world_;
};

#endif // TRANSFORM_H