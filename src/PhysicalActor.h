/*
 * PhysicalActor.h
 *
 *      Author: leo
 */

#ifndef SRC_PHYSICALACTOR_H_
#define SRC_PHYSICALACTOR_H_

#include <bullet/LinearMath/btMotionState.h>

#include "Actor.h"

class btRigidBody;

class PhysicalActor: public Actor, public btMotionState {
public:
public:
	PhysicalActor() {};
	virtual ~PhysicalActor() {};

	void update(float dt);
	Mesh *get_mesh() { return NULL; }

	void set_rigid_body(btRigidBody *rigid_body) { rigid_body_ = rigid_body; }
	btRigidBody *rigid_body() const { return rigid_body_; }

	virtual void getWorldTransform(btTransform& wt) const;
	virtual void setWorldTransform(const btTransform& wt);

protected:

	btRigidBody *rigid_body_;
};

#endif /* SRC_PHYSICALACTOR_H_ */
