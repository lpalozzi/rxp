/*
 * StaticMeshNode.h
 *
 *      Author: leo
 */

#ifndef SRC_STATICMESHNODE_H_
#define SRC_STATICMESHNODE_H_

#include "SceneNode.h"

class Mesh;

class StaticMeshNode: public SceneNode {
public:
	StaticMeshNode();
	StaticMeshNode(Mesh *mesh);
	virtual ~StaticMeshNode();

	Mesh *get_mesh() const { return mesh_; }

protected:

	Mesh *mesh_;
};

#endif /* SRC_STATICMESHNODE_H_ */
