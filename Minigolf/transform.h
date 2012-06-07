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
	Transform() 
		: scale_(1.0f),
		  right_(1.0f, 0.0f, 0.0f),
		  up_(0.0f, 1.0f, 0.0f),
		  forward_(0.0f, 0.0f, 1.0f) { }

	mat4 world() const { return world_; }

	vec3 position() const { return position_; }
	vec3 scale() const { return scale_; }
	quat rotation() const { return rotation_; }

	vec3 right() const { return right_; }
	vec3 up() const { return up_; }
	vec3 forward() const { return forward_; }

	void set_position(const vec3 &position);
	void set_scale(const vec3 &scale);
	void set_rotation(const quat &rotation);

	void Translate(const vec3 &translation);
	void Translate(const float &x, const float &y, const float &z);

	void LookAt(const Transform &target, const vec3 &up = vec3(0.0f, 1.0f, 0.0f));
	void LookAt(const vec3 &target, const vec3 &up = vec3(0.0f, 1.0f, 0.0f));

	// Todo: implement rotation
	void Rotate(const vec3 &euler_angles);
	void Rotate(const float &x, const float &y, const float &z);
	void Rotate(const vec3 &axis, const float &angle);
	
private:
	vec3 position_;
	vec3 scale_;
	quat rotation_;
	vec3 right_;
	vec3 up_;
	vec3 forward_;
	mat4 world_;

	void UpdateAxes();
	void UpdateMatrix();
};

typedef boost::shared_ptr<Transform> TransformPtr;

#endif // TRANSFORM_H