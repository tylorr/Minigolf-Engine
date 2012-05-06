#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "component.h"

using std::vector;

using glm::vec3;
using glm::quat;
using glm::mat4;

class Transform : public Component {
public:
	Transform *parent_;
	vector<Transform *> children_;

	Transform();
	~Transform();

	mat4 Local();
	mat4 World();

	// TODO: add translate, rotate, and scale functions
private:
	vec3 position_;
	vec3 scale_;
	quat rotation_;

	bool dirty_;
	mat4 world_;
	mat4 cached_parent_world_;
};

#endif // TRANSFORM_H