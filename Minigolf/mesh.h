#ifndef MESH_H
#define MESH_H

#include <boost\shared_ptr.hpp>

#include "component.h"

#include "material.h"
#include "geometry.h"

struct Mesh : public Component {
	boost::shared_ptr<Material> material;
	boost::shared_ptr<Geometry> geometry;

	Mesh() : Component("Mesh") { }
};

#endif // MESH_H