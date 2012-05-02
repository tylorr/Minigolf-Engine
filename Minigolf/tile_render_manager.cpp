#include "tile_render_manager.h"

#include "entity.h"

using namespace glm;
using namespace std;

vector<TileRenderComponent *> TileRenderManager::components;

Entity *TileRenderManager::CreateTile(const tile &t) {
	Entity *e = new Entity();

	TileRenderComponent *comp = new TileRenderComponent();
	comp->Initialize(t);

	components.push_back(comp);

	e->AddComponent(comp);

	return e;
}

void TileRenderManager::Render(stack<mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &NormalMatrixUnifromLocation) {
	vector<TileRenderComponent *>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->Render(ModelViewMatrix, ModelViewMatrixUniformLocation, NormalMatrixUnifromLocation);
	}
}