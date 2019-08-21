/*
 * Flipper.h
 *
 *      Author: leo
 */

#ifndef SRC_FLIPPER_H_
#define SRC_FLIPPER_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "Actor.h"

class btCollisionShape;

class Flipper: public Actor, public btMotionState
{
public:
	Flipper(Mesh *mesh) : mesh_(mesh) {};
	virtual ~Flipper() {};

	void update(float dt);
	Mesh *get_mesh() { return mesh_; }

	btRigidBody *rigid_body() const { return rigid_body_; }
	void setup_rigid_body(btCollisionShape *collision_shape);

	virtual void getWorldTransform(btTransform& wt) const;
	virtual void setWorldTransform(const btTransform& wt);

protected:

	Mesh *mesh_;
	btRigidBody *rigid_body_;
};

#endif /* SRC_FLIPPER_H_ */
