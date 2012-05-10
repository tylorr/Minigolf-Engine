#include "material.h"
#include "entity.h"
#include "camera.h"
#include "mesh.h"

#include "renderer.h"

Renderer::Renderer(Camera *camera, Mesh *mesh, Material *material) {
	camera_ = camera;
	mesh_ = mesh;
	material_ = material;
}

Renderer::~Renderer() {
}

void Renderer::Render() const {
	// get camera view matrix
	// get camera projection matrix (p)
	// get entity model matrix
	// calculate mv (view * model)
	// calculate mvp (p * mv)
	// calculate normal matrix (inverse(transpose(mv)))
	// attach + push material to gpu
	// push matrices to gpu
	// draw mesh
	// release material

	using glm::inverse;
	using glm::transpose;

	mat4 view = camera_->entity_->transform_.World();
	mat4 projection = camera_->Projection();
	mat4 model = entity_->transform_.World();
	mat4 model_view = view * model;
	mat4 mvp = projection * model_view;
	mat4 normal = inverse(transpose(model_view));

	material_->PreRender();

	material_->PushMatrices(model_view, projection, mvp, normal);
	mesh_->Draw();

	material_->PostRender();
}