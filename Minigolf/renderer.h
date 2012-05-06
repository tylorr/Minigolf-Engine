#ifndef RENDERER_H
#define RENDERER_H

#include "component.h"

class Material;
class Mesh;
class Camera;

class Renderer : public Component {
public:
	Camera *camera_;
	Mesh *mesh_;
	Material *material_;

	Renderer(Camera *camera, Mesh *mesh, Material *material);
	~Renderer();

	void Render() const;
};

#endif // RENDERER_H