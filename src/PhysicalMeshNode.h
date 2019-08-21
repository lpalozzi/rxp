/*
 * PhysicalMeshNode.h
 *
 *  Created on: 2018-04-09
 *      Author: leo
 */

#ifndef SRC_PHYSICALMESHNODE_H_
#define SRC_PHYSICALMESHNODE_H_

#include <bullet/LinearMath/btMotionState.h>

#include "StaticMeshNode.h"

class btRigidBody;
class Mesh;

class PhysicalMeshNode: public StaticMeshNode, public btMotionState {
public:
	PhysicalMeshNode();
	PhysicalMeshNode(btRigidBody *rigid_body, Mesh *mesh);
	virtual ~PhysicalMeshNode();

	virtual void getWorldTransform(btTransform& wt) const;
	virtual void setWorldTransform(const btTransform& wt);

protected:

	btRigidBody *rigid_body_;
};

#endif /* SRC_PHYSICALMESHNODE_H_ */
