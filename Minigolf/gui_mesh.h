#ifndef GUI_MESH_H
#define GUI_MESH_H

#include "mesh.h"

struct GuiMesh : public Mesh { };

typedef boost::shared_ptr<GuiMesh> GuiMeshPtr;

#endif // GUI_MESH_H