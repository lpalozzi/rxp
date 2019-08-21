/*
 * BallObject.h
 *
 *      Author: leo
 */

#ifndef BALLOBJECT_H_
#define BALLOBJECT_H_

#include "PhysicalMeshNode.h"

class Pinball: public PhysicalMeshNode {
public:
	Pinball();
	Pinball(btRigidBody *rigid_body, Mesh *mesh);
	virtual ~Pinball() {};

	static constexpr float RADIUS = 1.35f;

	void setup_rigid_body();

protected:


};

#endif /* BALLOBJECT_H_ */
