/*
 * StaticMesh.h
 *
 *      Author: leo
 */

#ifndef SRC_STATICMESH_H_
#define SRC_STATICMESH_H_

#include "Actor.h"

class StaticMesh: public Actor {
public:
	StaticMesh(Mesh *mesh) : mesh_(mesh) {};
	virtual ~StaticMesh() {};

	void update(float dt) {};
	Mesh *get_mesh() { return mesh_; }

protected:

	Mesh *mesh_;
};

#endif /* SRC_STATICMESH_H_ */
