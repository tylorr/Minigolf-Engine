#ifndef TILE_RENDER_MANAGER_H
#define TILE_RENDER_MANAGER_H

#include <vector>
#include <stack>

#include "Utils.h"

#include "tile_render_component.h"
#include "file_handling.h"

using glm::mat4;

class Entity;

class TileRenderManager {
public:
	

	static void Render(std::stack<mat4> *ModelViewMatrix, const mat4 &ProjectionMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &ProjectionMatrixUniform, const GLuint &NormalMatrixUnifromLocation);

	static Entity *CreateTile(const Tile &t);	

private:
	static std::vector<TileRenderComponent *> components;
};

#endif // TILE_RENDER_MANAGER_H