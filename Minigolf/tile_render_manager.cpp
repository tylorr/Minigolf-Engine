#include "tile_render_manager.h"

#include "entity.h"


using std::vector;
using std::stack;

vector<TileRenderComponent *> TileRenderManager::components;

Entity *TileRenderManager::CreateTile(const Tile &t) {
	Entity *e = new Entity();

	TileRenderComponent *comp = new TileRenderComponent();
	comp->Initialize(t);

	components.push_back(comp);

	e->AddComponent(comp);

	return e;
}

void TileRenderManager::Render(stack<mat4> *ModelViewMatrix, const mat4 &ProjectionMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &ProjectionMatrixUniform, const GLuint &NormalMatrixUnifromLocation) {
	vector<TileRenderComponent *>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->Render(ModelViewMatrix, ProjectionMatrix, ModelViewMatrixUniformLocation, ProjectionMatrixUniform, NormalMatrixUnifromLocation);
	}
}