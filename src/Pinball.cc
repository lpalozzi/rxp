/*
 * BallObject.cpp
 *
 *      Author: leo
 */

#include <bullet/btBulletDynamicsCommon.h>

#if 0
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#endif

#include "Pinball.h"

Pinball::Pinball(btRigidBody *rigid_body, Mesh *mesh)
{
	PhysicalMeshNode(rigid_body, mesh);
}


void Pinball::setup_rigid_body()
{
	btCollisionShape* ballShape = new btSphereShape(Pinball::RADIUS);
    btScalar mass = .4;
    btVector3 ballInertia(0, 0, 0);
    ballShape->calculateLocalInertia(mass, ballInertia);
    ballShape->setMargin(0.0f);
    btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(mass, this, ballShape, ballInertia);
    //ballRigidBodyCI.m_restitution = 100.1;
    //ballRigidBodyCI.m_friction = 1.5;
    rigid_body_ = new btRigidBody(ballRigidBodyCI);
    rigid_body_->setRestitution(1.0);
    rigid_body_->setFriction(0.0);
    //rigid_body_->setActivationState(DISABLE_DEACTIVATION);
    rigid_body_->setCcdMotionThreshold(1e-7);
    rigid_body_->setCcdSweptSphereRadius(Pinball::RADIUS);
}

#if 0
void Pinball::getWorldTransform(btTransform& wt) const
{
	glm::mat4 transform = get_transform();
	glm::vec3 pos = transform[3];
	wt.setOrigin(btVector3(pos.x, pos.y, pos.z));
	wt.setRotation(btQuaternion(0,0,0,1));
}

void Pinball::setWorldTransform(const btTransform& bt_world_transform)
{
	btVector3 bt_pos = bt_world_transform.getOrigin();
	btQuaternion bt_rot = bt_world_transform.getRotation();
	glm::quat qrot = glm::quat((float)bt_rot.w(), (float)bt_rot.x(), (float)bt_rot.y(), (float)bt_rot.z());
	set_transform(glm::translate(glm::mat4(1), glm::vec3(bt_pos.x(), bt_pos.y(), bt_pos.z())) * glm::mat4_cast(qrot));
}
#endif

