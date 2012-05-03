#ifndef TILE_RENDER_MANAGER_H
#define TILE_RENDER_MANAGER_H

#include <vector>
#include <stack>

#include "Utils.h"

#include "tile_render_component.h"
#include "file_handling.h"

class Entity;

class TileRenderManager {
public:
	

	static void Render(std::stack<glm::mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &NormalMatrixUnifromLocation);

	static Entity *CreateTile(const tile &t);	

private:
	static std::vector<TileRenderComponent *> components;
};

#endif // TILE_RENDER_MANAGER_H